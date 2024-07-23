#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "contrib/heap.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"
#include "TCPHeaderData.h"
#include "getSendWindowSize.h"
#include "computeTCPDataOffset.h"
#include "writeTCPHeader.h"
#include "checkByteInWindow.h"
#include "sendTCPPacket.h"
#include "tcpRetransmissionTimerCallback.h"
#include "getMonotonicTimeval.h"
#include "addTimeval.h"
#include "enqueueTimeout.h"
#include "startTimer.h"
#include "retry_delay.h"
#include "HEADERS_RESERVE.h"

#include "processTCPData.h"
unsigned int processTCPData(struct CaptureContext *context, uint8_t *packet, unsigned int count, void *arg) {
	struct TCPConnection *connection;
	connection = (struct TCPConnection *) arg;
	assert(context == connection->context);
	struct TCPHeaderData header;
	header.src_port = connection->strategy->port_getter(&connection->addrs.dst);
	header.dst_port = connection->strategy->port_getter(&connection->addrs.src);
	header.seq_num = connection->our_seq;
	connection->our_seq += count;
	header.ack_num = connection->first_desired;
	header.urg = header.psh = header.rst = header.syn = header.fin = false;
	header.ack = true;
	header.raw_window = getSendWindowSize(connection);
	header.urgent_ptr = 0;
	header.mss_present = header.winscale_present = false;
	header.data_offset = computeTCPDataOffset(&header);
	uint8_t *pseudo;
	pseudo = alloca(connection->strategy->pseudo_length);
	connection->strategy->create_pseudo(pseudo, &connection->addrs.dst, &connection->addrs.src, 6, header.data_offset + count);
	writeTCPHeader(packet, count, &header, pseudo, connection->strategy->pseudo_length, &connection->mutex);
	unsigned int frags;
	frags = connection->strategy->compute_fragcount(header.data_offset + count, connection->context->settings->mtu);
	assert(frags == 1);
	struct IPFragmentMetadata metadata;
	connection->strategy->fill_metadatas(&metadata, frags, header.data_offset + count, connection->context->settings->mtu);
	metadata.buffer = &packet[(signed int) -(header.data_offset + metadata.header_size)];
	connection->strategy->write_headers(context, &metadata, frags, 6, &connection->addrs.dst, &connection->addrs.src);
	struct TCPAppQueueItem *item;
	item = malloc(sizeof(struct TCPAppQueueItem));
	if (NULL == item) {
		pthread_mutex_unlock(&connection->mutex);
		return 1;
	};
	item->ip_packet = &packet[(signed int) -(header.data_offset + metadata.header_size)];
	item->ip_size = header.data_offset + metadata.header_size + count;
	item->tcp_packet = &packet[(signed int) -header.data_offset];
	item->tcp_size = header.data_offset + count;
	item->data_size = count;
	item->confirm_ack = header.seq_num + count;
	item->connection = connection;
	item->free_me = &packet[-HEADERS_RESERVE];
	item->next = NULL;
	if (checkByteInWindow(connection->latest_ack, connection->app_window, item->confirm_ack - item->data_size) && checkByteInWindow(connection->latest_ack, connection->app_window, item->confirm_ack)) sendTCPPacket(connection, item, false);
	struct timeval now, expire;
	getMonotonicTimeval(&now);
	addTimeval(&now, &retry_delay, &expire);
	item->timeout = enqueueTimeout(context, &expire, &tcpRetransmissionTimerCallback, item);
	*(connection->app_last) = item;
	connection->app_last = &(item->next);
	pthread_mutex_lock(&context->timeout_mutex);
	startTimer(context);
	pthread_mutex_unlock(&context->timeout_mutex);
	pthread_mutex_unlock(&connection->mutex);
	return 0;
};

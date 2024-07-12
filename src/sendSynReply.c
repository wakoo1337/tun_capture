#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "contrib/heap.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "TCPAppQueueItem.h"
#include "TCPHeaderData.h"
#include "TCPConnection.h"
#include "getSendWindowSize.h"
#include "writeTCPHeader.h"
#include "startTimer.h"
#include "enqueueTimeout.h"
#include "tcpRetransmissionTimerCallback.h"
#include "sendTCPPacket.h"
#include "getMonotonicTimeval.h"
#include "addTimeval.h"
#include "computeTCPDataOffset.h"
#include "retry_delay.h"
#include "HEADERS_RESERVE.h"

#include "sendSynReply.h"
unsigned int sendSynReply(struct TCPConnection *connection) {
	struct TCPHeaderData hdr;
	hdr.src_port = connection->strategy->port_getter(&connection->addrs.dst);
	hdr.dst_port = connection->strategy->port_getter(&connection->addrs.src);
	hdr.seq_num = connection->our_seq;
	hdr.ack_num = connection->first_desired;
	hdr.urg = hdr.psh = hdr.rst = hdr.fin = false;
	hdr.ack = hdr.syn = true;
	hdr.raw_window = getSendWindowSize(connection);
	hdr.urgent_ptr = 0;
	hdr.mss_present = true;
	hdr.mss_value = connection->strategy->compute_mss(connection->context);
	hdr.winscale_present = connection->scaling_enabled;
	hdr.winscale_value = connection->our_scale;
	uint8_t *packet;
	packet = malloc(HEADERS_RESERVE * sizeof(uint8_t));
	if (NULL == packet) return 1;
	uint8_t *pseudo;
	pseudo = alloca(connection->strategy->pseudo_length);
	hdr.data_offset = computeTCPDataOffset(&hdr);
	connection->strategy->create_pseudo(pseudo, &connection->addrs.dst, &connection->addrs.src, 6, hdr.data_offset);
	writeTCPHeader(&packet[HEADERS_RESERVE], 0, &hdr, pseudo, connection->strategy->pseudo_length);
	unsigned int frags;
	frags = connection->strategy->compute_fragcount(hdr.data_offset, connection->context->settings->mtu);
	assert(frags == 1);
	struct IPFragmentMetadata metadata;
	connection->strategy->fill_metadatas(&metadata, frags, hdr.data_offset, connection->context->settings->mtu);
	metadata.buffer = &packet[HEADERS_RESERVE-hdr.data_offset-metadata.header_size];
	connection->strategy->write_headers(connection->context, &metadata, frags, 6, &connection->addrs.dst, &connection->addrs.src);
	struct TCPAppQueueItem *queue_item;
	queue_item = malloc(sizeof(struct TCPAppQueueItem));
	if (NULL == queue_item) {
		free(packet);
		return 1;
	};
	queue_item->ip_packet = &packet[HEADERS_RESERVE-metadata.header_size-metadata.data_size];
	queue_item->ip_size = metadata.header_size + metadata.data_size;
	queue_item->tcp_packet = &packet[HEADERS_RESERVE-metadata.data_size];
	queue_item->tcp_size = metadata.data_size;
	struct timeval now;
	getMonotonicTimeval(&now);
	addTimeval(&now, &retry_delay, &queue_item->retry);
	queue_item->confirm_ack = hdr.seq_num + 1;
	queue_item->free_me = packet;
	queue_item->next = NULL;
	*connection->app_last = queue_item;
	connection->app_last = &queue_item->next;
	sendTCPPacket(connection->context, queue_item);
	connection->timer = enqueueTimeout(connection->context, &queue_item->retry, &tcpRetransmissionTimerCallback, connection);
	pthread_mutex_lock(&connection->context->timeout_mutex);
	startTimer(connection->context);
	pthread_mutex_unlock(&connection->context->timeout_mutex);
	return 0;
};
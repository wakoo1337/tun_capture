#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/socket.h>
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
#include "enqueueTCPPacketTransmission.h"
#include "enqueueTCPRetransmission.h"
#include "isAppQueueItemInWindow.h"
#include "freeNoRefsAppQueueItem.h"
#include "findPreviousNextAppQueueItem.h"
#include "decrementAppQueueItemRefCount.h"
#include "HEADERS_RESERVE.h"

#include "processTCPData.h"
unsigned int processTCPData(struct CaptureContext *context, uint8_t *packet, unsigned int count, void *arg) {
	struct TCPConnection *connection;
	connection = (struct TCPConnection *) arg;
	assert(context == connection->context);
	struct TCPAppQueueItem *item;
	item = malloc(sizeof(struct TCPAppQueueItem));
	if (NULL == item) return 1;
	item->is_filled = false;
	item->next = NULL;
	*connection->app_last = item;
	connection->app_last = &item->next;
	struct TCPHeaderData header;
	header.src_port = connection->strategy->port_getter(&connection->addrs.dst);
	header.dst_port = connection->strategy->port_getter(&connection->addrs.src);
	header.seq_num = connection->our_seq;
	header.ack_num = connection->first_desired;
	header.urg = header.rst = header.syn = header.fin = false;
	header.ack = true;
	header.psh = (count != 0) && (count < connection->max_pktdata);
	header.raw_window = getSendWindowSize(connection);
	header.urgent_ptr = 0;
	header.mss_present = header.winscale_present = false;
	header.data_offset = computeTCPDataOffset(&header);
	uint8_t pseudo[connection->strategy->pseudo_length];
	connection->strategy->create_pseudo(pseudo, &connection->addrs.dst, &connection->addrs.src, 6, header.data_offset + count);
	connection->our_seq += count;
	writeTCPHeader(packet, count, &header, pseudo, connection->strategy->pseudo_length, &connection->mutex);
	unsigned int frags;
	frags = connection->strategy->compute_fragcount(header.data_offset + count, connection->context->settings->mtu);
	assert(frags == 1);
	struct IPFragmentMetadata metadata;
	connection->strategy->fill_metadatas(&metadata, frags, header.data_offset + count, connection->context->settings->mtu);
	metadata.buffer = &packet[(signed int) -(header.data_offset + metadata.header_size)];
	connection->strategy->write_headers(context, &metadata, frags, 6, &connection->addrs.dst, &connection->addrs.src);
	item->ip_packet = &packet[(signed int) -(header.data_offset + metadata.header_size)];
	item->ip_size = header.data_offset + metadata.header_size + count;
	item->data_size = count;
	item->confirm_ack = header.seq_num + count;
	item->connection = connection;
	item->timeout = NULL;
	item->free_me = &packet[-HEADERS_RESERVE];
	item->is_filled = true;
	item->ref_count = 2; // Одна ссылка — в связном списке, вторая — в локальной переменной в этой функции
	if (isAppQueueItemInWindow(connection->latest_ack, connection->app_window, item)) {
		if (enqueueTCPPacketTransmission(item) || enqueueTCPRetransmission(item)) {
			struct TCPAppQueueItem **previous_next = findPreviousNextAppQueueItem(connection, item);
			if (previous_next) *previous_next = item->next;
			if (connection->app_last == &item->next) {
				if (previous_next) connection->app_last = previous_next;
				else {
					connection->app_queue = NULL;
					connection->app_last = &connection->app_queue;
				};
			};
			free(item->free_me);
			free(item);
			return 1;
		};
	};
	decrementAppQueueItemRefCount(item); // Ссылка из локальной переменной ушла, всё.
	freeNoRefsAppQueueItem(item);
	return 0;
};

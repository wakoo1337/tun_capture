#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "contrib/logdel_heap.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "TCPHeaderData.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"
#include "writeTCPHeader.h"
#include "enqueueTCPPacketTransmission.h"
#include "getSendWindowSize.h"
#include "computeTCPDataOffset.h"
#include "enqueueTCPRetransmission.h"
#include "freeNoRefsAppQueueItem.h"
#include "decrementAppQueueItemRefCount.h"
#include "findPreviousNextAppQueueItem.h"
#include "HEADERS_RESERVE.h"

#include "sendTCPFinalize.h"
unsigned int sendTCPFinalize(struct TCPConnection *connection) {
	struct TCPHeaderData header;
	header.src_port = connection->strategy->port_getter(&connection->addrs.dst);
	header.dst_port = connection->strategy->port_getter(&connection->addrs.src);
	header.seq_num = connection->our_seq;
	header.ack_num = connection->first_desired;
	header.urg = header.psh = header.rst = header.syn = false;
	header.ack = header.fin = true;
	header.raw_window = getSendWindowSize(connection);
	header.urgent_ptr = 0;
	header.mss_present = header.winscale_present = false;
	header.data_offset = computeTCPDataOffset(&header);
	uint8_t pseudo[connection->strategy->pseudo_length];
	connection->strategy->create_pseudo(pseudo, &connection->addrs.dst, &connection->addrs.src, 6, header.data_offset);
	uint8_t *packet;
	packet = malloc(HEADERS_RESERVE * sizeof(uint8_t));
	if (NULL == packet) return 1;
	writeTCPHeader(&packet[HEADERS_RESERVE], 0, &header, pseudo, connection->strategy->pseudo_length, &connection->mutex);
	unsigned int frags;
	frags = connection->strategy->compute_fragcount(header.data_offset, connection->context->settings->mtu);
	assert(frags == 1);
	struct IPFragmentMetadata metadata;
	connection->strategy->fill_metadatas(&metadata, frags, header.data_offset, connection->context->settings->mtu);
	metadata.buffer = &packet[HEADERS_RESERVE - header.data_offset - metadata.header_size];
	connection->strategy->write_headers(connection->context, &metadata, frags, 6, &connection->addrs.dst ,&connection->addrs.src);
	struct TCPAppQueueItem *queue_item;
	queue_item = malloc(sizeof(struct TCPAppQueueItem));
	if (NULL == queue_item) {
		free(packet);
		return 1;
	};
	queue_item->ip_packet = &packet[HEADERS_RESERVE - header.data_offset - metadata.header_size];
	queue_item->ip_size = header.data_offset + metadata.header_size;
	queue_item->data_size = 0;
	queue_item->confirm_ack = header.seq_num + 1;
	queue_item->connection = connection;
	queue_item->timeout = NULL;
	queue_item->free_me = packet;
	queue_item->ref_count = 2; // Одна ссылка — в связном списке, вторая — в локальной переменной в этой функции
	queue_item->next = NULL;
	queue_item->is_filled = true;
	*connection->app_last = queue_item;
	connection->app_last = &queue_item->next;
	if (enqueueTCPPacketTransmission(queue_item) || enqueueTCPRetransmission(queue_item)) {
		struct TCPAppQueueItem **previous_next = findPreviousNextAppQueueItem(connection, queue_item);
		if (previous_next) *previous_next = queue_item->next;
		if (connection->app_last == &queue_item->next) {
			if (previous_next) connection->app_last = previous_next;
			else {
				connection->app_queue = NULL;
				connection->app_last = &connection->app_queue;
			};
		};
		free(queue_item->free_me);
		free(queue_item);
		return 1;
	};
	decrementAppQueueItemRefCount(queue_item);
	freeNoRefsAppQueueItem(queue_item);
	return 0;
};

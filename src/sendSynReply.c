#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/time.h>
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
#include "enqueueTCPPacketTransmission.h"
#include "computeTCPDataOffset.h"
#include "enqueueTCPRetransmission.h"
#include "decrementAppQueueItemRefCount.h"
#include "freeNoRefsAppQueueItem.h"
#include "HEADERS_RESERVE.h"

#include "sendSynReply.h"
unsigned int sendSynReply(struct TCPConnection *connection) {
	struct TCPHeaderData hdr;
	hdr.src_port = connection->strategy->port_getter(&connection->addrs.dst);
	hdr.dst_port = connection->strategy->port_getter(&connection->addrs.src);
	hdr.seq_num = connection->seq_next;
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
	uint8_t pseudo[connection->strategy->pseudo_length];
	hdr.data_offset = computeTCPDataOffset(&hdr);
	connection->strategy->create_pseudo(pseudo, &connection->addrs.dst, &connection->addrs.src, 6, hdr.data_offset);
	writeTCPHeader(&packet[HEADERS_RESERVE], 0, &hdr, pseudo, connection->strategy->pseudo_length, &connection->mutex);
	unsigned int frags;
	frags = connection->strategy->compute_fragcount(hdr.data_offset, connection->context->settings->mtu);
	assert(frags == 1);
	struct IPFragmentMetadata metadata;
	connection->strategy->fill_metadatas(&metadata, frags, hdr.data_offset, connection->context->settings->mtu);
	metadata.buffer = &packet[HEADERS_RESERVE-hdr.data_offset-metadata.header_size];
	connection->strategy->write_headers(connection->context, &metadata, frags, 6, &connection->addrs.dst, &connection->addrs.src);
	struct TCPAppQueueItem *queue_item;
	queue_item = malloc(sizeof * queue_item);
	if (NULL == queue_item) {
		free(packet);
		return 1;
	};
	queue_item->ip_packet = &packet[HEADERS_RESERVE-metadata.header_size-metadata.data_size];
	queue_item->ip_size = metadata.header_size + metadata.data_size;
	queue_item->data_size = 0;
	queue_item->confirm_ack = connection->seq_next + 1;
	queue_item->connection = connection;
	queue_item->timeout = NULL;
	queue_item->free_me = packet;
	queue_item->ref_count = 2;
	queue_item->next = NULL;
	*connection->app_last = queue_item;
	connection->app_last = &queue_item->next;
	if (enqueueTCPPacketTransmission(queue_item)) {
		return 1; // TODO обработать ошибки
	};
	if (enqueueTCPRetransmission(queue_item)) {
		return 1; // TODO обработать ошибки
	};
	decrementAppQueueItemRefCount(queue_item);
	freeNoRefsAppQueueItem(queue_item);
	return 0;
};

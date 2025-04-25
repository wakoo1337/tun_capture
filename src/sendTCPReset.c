#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "TCPHeaderData.h"
#include "IPPacketPayload.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "SrcDstSockaddrs.h"
#include "PacketQueueItem.h"
#include "HEADERS_RESERVE.h"
#include "computeTCPDataOffset.h"
#include "writeTCPHeader.h"
#include "enqueueTxPacket.h"
#include "sendPacketOnce.h"

#include "sendTCPReset.h"
unsigned int sendTCPReset(struct CaptureContext *context, const struct IPPacketPayload *payload, const struct NetworkProtocolStrategy *strategy, const struct SrcDstSockaddrs *addrs) {
	struct TCPHeaderData header;
	header.src_port = strategy->port_getter(&addrs->dst);
	header.dst_port = strategy->port_getter(&addrs->src);
	header.seq_num = 0;
	header.ack_num = 0;
	header.urg = header.ack = header.psh = header.syn = header.fin = false;
	header.rst = true;
	header.raw_window = 0;
	header.urgent_ptr = 0;
	header.mss_present = header.winscale_present = false;
	header.data_offset = computeTCPDataOffset(&header);
	uint8_t pseudo[strategy->pseudo_length];
	strategy->create_pseudo(pseudo, &addrs->dst, &addrs->src, 6, header.data_offset);
	uint8_t *packet;
	packet = malloc(HEADERS_RESERVE);
	if (NULL == packet) return 1;
	writeTCPHeader(&packet[HEADERS_RESERVE], 0, &header, pseudo, strategy->pseudo_length, NULL);
	unsigned int frags;
	frags = strategy->compute_fragcount(header.data_offset, context->settings->mtu);
	assert(frags == 1);
	struct IPFragmentMetadata metadata;
	strategy->fill_metadatas(&metadata, frags, header.data_offset, context->settings->mtu);
	metadata.buffer = &packet[HEADERS_RESERVE - header.data_offset - metadata.header_size];
	strategy->write_headers(context, &metadata, frags, 6, &addrs->dst, &addrs->src);
	struct PacketQueueItem *queue_item;
	queue_item = malloc(sizeof(struct PacketQueueItem));
	if (NULL == queue_item) {
		free(packet);
		return 1;
	};
	queue_item->data = metadata.buffer;
	queue_item->count = metadata.header_size + header.data_offset;
	queue_item->processor = &sendPacketOnce;
	queue_item->mutex = NULL;
	queue_item->semaphore = NULL;
	queue_item->free_me = packet;
	queue_item->arg = queue_item;
	return enqueueTxPacket(context, queue_item);
};

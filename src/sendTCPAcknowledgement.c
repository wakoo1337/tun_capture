#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "TCPHeaderData.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "PacketQueueItem.h"
#include "writeTCPHeader.h"
#include "getSendWindowSize.h"
#include "computeTCPDataOffset.h"
#include "sendPacketOnce.h"
#include "enqueueTxPacket.h"
#include "HEADERS_RESERVE.h"

#include "sendTCPAcknowledgement.h"
unsigned int sendTCPAcknowledgement(struct TCPConnection *connection) {
	struct TCPHeaderData header;
	header.src_port = connection->strategy->port_getter(&connection->addrs.dst);
	header.dst_port = connection->strategy->port_getter(&connection->addrs.src);
	header.seq_num = connection->seq_next;
	header.ack_num = connection->first_desired;
	header.urg = header.psh = header.rst = header.syn = header.fin = false;
	header.ack = true;
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
	connection->strategy->write_headers(connection->context, &metadata, frags, 6, &connection->addrs.dst, &connection->addrs.src);
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
	return enqueueTxPacket(connection->context, queue_item);
};

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "contrib/avl.h"
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
#include "enqueueTCPPacketTransmission.h"
#include "enqueueTCPRetransmission.h"
#include "isAppQueueItemInWindow.h"
#include "freeNoRefsAppQueueItem.h"
#include "sendTCPFinalize.h"
#include "incrementAppQueueItemRefCount.h"
#include "decrementAppQueueItemRefCount.h"
#include "enqueueUnsentTCPPacketsTransmission.h"
#include "HEADERS_RESERVE.h"

#include "processTCPData.h"
unsigned int processTCPData(struct CaptureContext *context, uint8_t *packet, unsigned int count, void *arg) {
	struct TCPConnection *connection;
	connection = (struct TCPConnection *) arg;
	assert(context == connection->context);
	struct TCPHeaderData header;
	header.src_port = connection->strategy->port_getter(&connection->addrs.dst);
	header.dst_port = connection->strategy->port_getter(&connection->addrs.src);
	header.seq_num = connection->seq_next;
	header.ack_num = connection->first_desired;
	connection->seq_next += count;
	header.urg = header.rst = header.syn = header.fin = false;
	header.ack = true;
	header.psh = (count != 0) && (count < connection->max_pktdata);
	header.raw_window = getSendWindowSize(connection);
	header.urgent_ptr = 0;
	header.mss_present = header.winscale_present = false;
	header.data_offset = computeTCPDataOffset(&header);
	uint8_t pseudo[connection->strategy->pseudo_length];
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
	if (NULL == item) return 1;
	item->seq = header.seq_num;
	item->ip_packet = &packet[(signed int) -(header.data_offset + metadata.header_size)];
	item->ip_size = header.data_offset + metadata.header_size + count;
	item->data_size = count;
	item->confirm_ack = header.seq_num + count;
	item->connection = connection;
	item->timeout = NULL;
	item->free_me = &packet[-HEADERS_RESERVE];
	item->next = NULL;
	item->ref_count = 1; // Одна ссылка в локальной переменной
	if ((item->seq == connection->seq_first) && isAppQueueItemInWindow(connection->latest_ack, connection->app_window, item)) {
		// Если наш элемент — следующий в очереди, то вставляем его в очередь
		connection->seq_first += item->data_size;
		*(connection->app_last) = item;
		connection->app_last = &item->next;
		if (enqueueTCPPacketTransmission(item)) {
		};
		incrementAppQueueItemRefCount(item);
		if (enqueueTCPRetransmission(item)) {
		}; // TODO обработать ошибку, удалить элемент
	} else {
		// Иначе, вставляем его в предочередь и вынимаем из неё те элементы, что до нашего
		void **probe;
		probe = avl_probe(connection->app_prequeue, item);
		if ((*probe) != item) {
			// TODO обработать ошибку
		};
		incrementAppQueueItemRefCount(item);
	};
	decrementAppQueueItemRefCount(item); // Ссылка из локальной переменной ушла, всё.
	freeNoRefsAppQueueItem(item);
	enqueueUnsentTCPPacketsTransmission(connection);
	if (connection->should_send_fin && (connection->fin_seq == connection->seq_first)) {
		sendTCPFinalize(connection);
		connection->seq_next++;
		connection->seq_first++;
	};
	return 0;
};

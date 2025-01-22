#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "PacketQueueItem.h"
#include "processTCPData.h"
#include "enqueueRxPacket.h"
#include "HEADERS_RESERVE.h"

#include "enqueueStubTCPPacketQueueItem.h"
unsigned int enqueueStubTCPPacketQueueItem(struct TCPConnection *connection) {
	struct PacketQueueItem *item;
	pthread_mutex_unlock(&connection->mutex);
	item = malloc(sizeof *item);
	if (NULL == item) {
		pthread_mutex_lock(&connection->mutex);
		return 1;
	};
	uint8_t *stub_buffer;
	stub_buffer = malloc(HEADERS_RESERVE * sizeof(uint8_t));
	if (NULL == stub_buffer) {
		free(item);
		pthread_mutex_lock(&connection->mutex);
		return 1;
	};
	item->data = &stub_buffer[HEADERS_RESERVE];
	item->count = 0;
	item->processor = &processTCPData;
	item->mutex = &connection->mutex;
	item->free_me = stub_buffer;
	item->arg = connection;
	enqueueRxPacket(connection->context, item);
	pthread_mutex_lock(&connection->mutex);
	return 0;
};

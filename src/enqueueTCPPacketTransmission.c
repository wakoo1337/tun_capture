#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <event2/event.h>
#include <sys/socket.h>
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "TCPAppQueueItem.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "enqueueTxPacket.h"
#include "sendTCPPacketRefcounted.h"
#include "incrementAppQueueItemRefCount.h"

#include "enqueueTCPPacketTransmission.h"
unsigned int enqueueTCPPacketTransmission(struct TCPAppQueueItem *app_item) {
	struct PacketQueueItem *queue_item;
	queue_item = malloc(sizeof(struct PacketQueueItem));
	if (NULL == queue_item) return 1;
	queue_item->data = app_item->ip_packet;
	queue_item->count = app_item->ip_size;
	queue_item->processor = &sendTCPPacketRefcounted;
	queue_item->mutex = NULL;
	queue_item->free_me = NULL;
	queue_item->arg = app_item;
	unsigned int result;
	result = enqueueTxPacket(app_item->connection->context, queue_item);
	if (result) return 1;
	incrementAppQueueItemRefCount(app_item);
	return 0;
};

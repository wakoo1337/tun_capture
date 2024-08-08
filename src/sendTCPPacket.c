#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <event2/event.h>
#include <sys/socket.h>
#include "contrib/heap.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "TCPAppQueueItem.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "enqueueTxPacket.h"
#include "sendTCPPacketRefcounted.h"

#include "sendTCPPacket.h"
unsigned int sendTCPPacket(struct TCPConnection *connection, struct TCPAppQueueItem *app_item) {
	if (app_item->is_filled) {
		struct PacketQueueItem *packet_item;
		packet_item = malloc(sizeof(struct PacketQueueItem));
		if (NULL == packet_item) return 1;
		packet_item->data = app_item->ip_packet;
		packet_item->count = app_item->ip_size;
		packet_item->processor = &sendTCPPacketRefcounted;
		packet_item->mutex = NULL;
		packet_item->free_me = app_item->free_me;
		packet_item->arg = app_item;
		return enqueueTxPacket(connection->context, packet_item);
	} else return 0;
};

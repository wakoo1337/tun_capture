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
#include "tunCallback.h"

#include "sendTCPPacket.h"
unsigned int sendTCPPacket(struct TCPConnection *connection, struct TCPAppQueueItem *app_item, bool free_after) {
	struct PacketQueueItem *packet_item;
	packet_item = malloc(sizeof(struct PacketQueueItem));
	if (NULL == packet_item) return 1;
	packet_item->data = app_item->ip_packet;
	packet_item->count = app_item->ip_size;
	packet_item->processor = NULL;
	packet_item->mutex = NULL;
	packet_item->free_me = free_after ? app_item->free_me : NULL;
	packet_item->arg = NULL;
	pthread_mutex_unlock(&connection->mutex);
	pthread_mutex_lock(&connection->context->queue_mutex);
	pthread_mutex_lock(&connection->mutex);
	packet_item->next = connection->context->send_stack;
	connection->context->send_stack = packet_item;
	event_free(connection->context->iface_event);
	connection->context->iface_event = event_new(connection->context->event_base, connection->context->settings->fd_getter(connection->context->settings->user), EV_READ | EV_WRITE | EV_PERSIST, &tunCallback, connection->context);
	if (NULL == connection->context->iface_event) {
		pthread_mutex_unlock(&connection->context->queue_mutex);
		return 1;
	};
	if (-1 == event_add(connection->context->iface_event, NULL)) {
		pthread_mutex_unlock(&connection->context->queue_mutex);
		return 1;
	};
	pthread_mutex_unlock(&connection->context->queue_mutex);
	return 0;
};

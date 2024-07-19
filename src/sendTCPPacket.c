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
#include "tunCallback.h"

#include "sendTCPPacket.h"
unsigned int sendTCPPacket(struct CaptureContext *context, struct TCPAppQueueItem *app_item, bool free_after) {
	struct PacketQueueItem *packet_item;
	packet_item = malloc(sizeof(struct PacketQueueItem));
	if (NULL == packet_item) return 1;
	packet_item->data = app_item->ip_packet;
	packet_item->count = app_item->ip_size;
	packet_item->processor = NULL;
	packet_item->free_me = free_after ? app_item->free_me : NULL;
	packet_item->arg = NULL;
	pthread_mutex_lock(&context->queue_mutex);
	packet_item->next = context->send_stack;
	context->send_stack = packet_item;
	event_free(context->iface_event);
	context->iface_event = event_new(context->event_base, context->settings->fd_getter(context->settings->user), EV_READ | EV_WRITE | EV_PERSIST, &tunCallback, context);
	if (NULL == context->iface_event) {
		pthread_mutex_unlock(&context->queue_mutex);
		return 1;
	};
	if (-1 == event_add(context->iface_event, NULL)) {
		pthread_mutex_unlock(&context->queue_mutex);
		return 1;
	};
	pthread_mutex_unlock(&context->queue_mutex);
	return 0;
};

#include <event2/event.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <errno.h>
#include "contrib/heap.h"
#include "ForwardingMappingIPv4.h"
#include "ForwardingMappingIPv6.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "packetsProcessor.h"
#include "enqueuePacket.h"

#include "tunCallback.h"
void tunCallback(evutil_socket_t fd, short what, void *arg) {
	struct CaptureContext *context = (struct CaptureContext *) arg;
	if (what & EV_READ) {
		uint8_t *packet_buffer;
		packet_buffer = malloc(context->settings->mtu * sizeof(uint8_t));
		if (packet_buffer == NULL) {
			event_base_loopexit(context->event_base, NULL);
			return;
		};
		ssize_t readed;
		readed = context->settings->read_function(packet_buffer, context->settings->mtu, context->settings->user);
		if (readed == -1) {
			free(packet_buffer);
			event_base_loopexit(context->event_base, NULL);
			return;
		};
		packet_buffer = realloc(packet_buffer, readed * sizeof(uint8_t)); // Уменьшаем буфер под пакет
		struct PacketQueueItem *queue_item;
		queue_item = malloc(sizeof(struct PacketQueueItem));
		if (queue_item == NULL) {
			free(packet_buffer);
			event_base_loopexit(context->event_base, NULL);
			return;
		};
		queue_item->free_me = queue_item->data = packet_buffer;
		queue_item->count = readed;
		queue_item->processor = &packetsProcessor;
		queue_item->mutex = NULL;
		queue_item->arg = NULL;
		pthread_mutex_lock(&context->rx_mutex);
		enqueuePacket(context, queue_item);
		pthread_cond_signal(&context->rx_cond);
		pthread_mutex_unlock(&context->rx_mutex);
	};
	if (what & EV_WRITE) {
		pthread_mutex_lock(&context->tx_mutex);
		while (context->tx_stack) {
			struct PacketQueueItem *next;
			next = context->tx_stack->next;
			ssize_t result;
			result = context->settings->write_function(context->tx_stack->data, context->tx_stack->count, context->settings->user);
			if (-1 == result) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					break;
				} else {
					pthread_mutex_unlock(&context->tx_mutex);
					event_base_loopexit(context->event_base, NULL);
					return;
				};
			};
			free(context->tx_stack->free_me);
			free(context->tx_stack);
			context->tx_stack = next;
		};
		if (NULL == context->tx_stack) {
			event_free(context->iface_event);
			context->iface_event = event_new(context->event_base, context->settings->fd_getter(context->settings->user), EV_READ | EV_PERSIST, &tunCallback, context);
			if (NULL == context->iface_event) {
				return;
			};
			if (-1 == event_add(context->iface_event, NULL)) {
				return;
			};
		};
		pthread_mutex_unlock(&context->tx_mutex);
	};
};

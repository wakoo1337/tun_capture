#include <event2/event.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "PacketQueueItem.h"
#include "CaptureContext.h"
#include "CaptureSettings.h"

#include "tunCallback.h"
void tunCallback(evutil_socket_t fd, short what, void *arg) {
	struct CaptureContext *context = (struct CaptureContext *) arg;
	if (what & EV_READ) {
		unsigned char *packet_buffer;
		packet_buffer = malloc(context->settings->tun_mtu * sizeof(unsigned char));
		if (packet_buffer == NULL) event_base_loopexit(context->event_base, NULL);
		ssize_t readed;
		readed = read(context->settings->fd, packet_buffer, context->settings->tun_mtu);
		if (readed == -1) {
			free(packet_buffer);
			event_base_loopexit(context->event_base, NULL);
		};
		packet_buffer = realloc(packet_buffer, readed); // Уменьшаем буфер под пакет
		context->settings->packet_callback(readed, packet_buffer, false, context->settings->user);
		struct PacketQueueItem *queue_item;
		queue_item = malloc(sizeof(struct PacketQueueItem));
		if (queue_item == NULL) {
			free(packet_buffer);
			event_base_loopexit(context->event_base, NULL);
		};
		queue_item->data = packet_buffer;
		queue_item->count = readed;
		pthread_mutex_lock(&context->queue_mutex);
		queue_item->next = context->captured_stack;
		context->captured_stack = queue_item;
		pthread_mutex_unlock(&context->queue_mutex);
		pthread_cond_signal(&context->queue_cond);
	};
};

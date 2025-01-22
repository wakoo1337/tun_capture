#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <event2/event.h>
#include "contrib/logdel_heap.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "enqueueRxPacket.h"
#include "packetsProcessor.h"
#include "emergencyStop.h"

#include "tunReadCallback.h"
void tunReadCallback(evutil_socket_t fd, short what, void *arg) {
	struct CaptureContext *context = (struct CaptureContext *) arg;
	if (what & EV_READ) {
		uint8_t *buffer;
		buffer = malloc(context->settings->mtu * sizeof(uint8_t));
		if (buffer == NULL) emergencyStop(context);
		ssize_t readed;
		readed = context->settings->read_function(buffer, context->settings->mtu, context->settings->user);
		if (readed == -1) {
			free(buffer);
			emergencyStop(context);
			return;
		} else if (readed == 0) {
			free(buffer);
			return;
		};
		uint8_t *new_buffer;
		new_buffer = realloc(buffer, readed * sizeof(uint8_t)); // Уменьшаем буфер под пакет
		if (NULL == new_buffer) {
			free(buffer);
			emergencyStop(context);
			return;
		};
		struct PacketQueueItem *queue_item;
		queue_item = malloc(sizeof(struct PacketQueueItem));
		if (queue_item == NULL) {
			free(new_buffer);
			emergencyStop(context);
			return;
		};
		queue_item->free_me = queue_item->data = new_buffer;
		queue_item->count = readed;
		queue_item->processor = &packetsProcessor;
		queue_item->mutex = NULL;
		queue_item->arg = NULL;
		enqueueRxPacket(context, queue_item);
	};
};

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <event2/event.h>
#include "contrib/heap.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "RefcountBuffer.h"
#include "enqueueRxPacket.h"
#include "packetsProcessor.h"
#include "emergencyStop.h"
#include "createRefcountBuffer.h"
#include "decrementRefcount.h"
#include "resizeRefcountBuffer.h"

#include "tunReadCallback.h"
void tunReadCallback(evutil_socket_t fd, short what, void *arg) {
	struct CaptureContext *context = (struct CaptureContext *) arg;
	if (what & EV_READ) {
		struct RefcountBuffer *buffer;
		buffer = createRefcountBuffer(context->settings->mtu);
		if (buffer == NULL) event_base_loopexit(context->event_base, NULL);
		ssize_t readed;
		readed = context->settings->read_function(buffer->data, context->settings->mtu, context->settings->user);
		if (readed == -1) {
			decrementRefcount(&buffer);
			emergencyStop(context);
			return;
		};
		resizeRefcountBuffer(&buffer, readed);
		if (buffer == NULL) {
			emergencyStop(context);
			return;
		};
		struct PacketQueueItem *queue_item;
		queue_item = malloc(sizeof(struct PacketQueueItem));
		if (queue_item == NULL) {
			decrementRefcount(&buffer);
			emergencyStop(context);
			return;
		};
		queue_item->buffer = buffer;
		queue_item->processor = &packetsProcessor;
		queue_item->mutex = NULL;
		queue_item->arg = NULL;
		enqueueRxPacket(context, queue_item);
	};
};

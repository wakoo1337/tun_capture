#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <event2/event.h>
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "dequeueTxPacket.h"
#include "emergencyStop.h"

#include "tunWriteCallback.h" 
void tunWriteCallback(evutil_socket_t fd, short what, void *arg) {
	struct CaptureContext *context;
	context = (struct CaptureContext *) arg;
	if (what & EV_WRITE) {
		struct PacketQueueItem *item;
		while (dequeueTxPacket(context, &item)) {
			if (item->processor(context, item->data, item->count, item->arg)) {
				free(item);
				emergencyStop(context);
				return;
			};
			free(item);
		};
		pthread_mutex_lock(&context->tx_mutex);
		if ((NULL == context->tx_begin) && (-1 == event_del(context->tx_event))) event_base_loopbreak(context->event_base);
		pthread_mutex_unlock(&context->tx_mutex);
	};
};

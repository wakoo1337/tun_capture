#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <event2/event.h>
#include "contrib/heap.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "dequeueTxPacket.h"

#include "tunWriteCallback.h" 
void tunWriteCallback(evutil_socket_t fd, short what, void *arg) {
	struct CaptureContext *context;
	context = (struct CaptureContext *) arg;
	if (what & EV_WRITE) {
		struct PacketQueueItem *item;
		dequeueTxPacket(context, &item);
		while (item) {
			ssize_t result;
			result = context->settings->write_function(item->data, item->count, context->settings->user);
			if (-1 == result) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					break;
				};
			};
			free(item->free_me);
			free(item);
			dequeueTxPacket(context, &item);
		};
		pthread_mutex_lock(&context->tx_mutex);
		if ((NULL == context->tx_begin) && (-1 == event_del(context->tx_event))) event_base_loopbreak(context->event_base);
		pthread_mutex_unlock(&context->tx_mutex);
	};
};

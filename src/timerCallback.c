#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <event2/event.h>
#include "contrib/heap.h"
#include "TimeoutItem.h"
#include "CaptureContext.h"
#include "getMonotonicTimeval.h"
#include "compareTimeval.h"
#include "startTimer.h"

#include "timerCallback.h"
void timerCallback(evutil_socket_t socket, short what, void *arg) {
	struct CaptureContext *context = (struct CaptureContext *) arg;
	if (what & EV_TIMEOUT) {
		pthread_mutex_lock(&context->timeout_mutex);
		struct timeval tv;
		getMonotonicTimeval(&tv);
		struct TimeoutItem *item;
		while ((item = heap_peek(context->timeout_queue)),
			((item != NULL) && (item->is_del || (compareTimeval(&item->expiration, &tv) <= 0)))) {
			item = heap_poll(context->timeout_queue);
			if (!item->is_del) {	
				pthread_mutex_lock(item->mutex);
				pthread_mutex_unlock(&context->timeout_mutex);
				item->callback(item->arg);
				pthread_mutex_unlock(item->mutex);
				pthread_mutex_lock(&context->timeout_mutex);
			};
			free(item);
			getMonotonicTimeval(&tv);
		};
		startTimer(context);
		pthread_mutex_unlock(&context->timeout_mutex);
	};
};

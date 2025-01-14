#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <event2/event.h>
#include "contrib/logdel_heap.h"
#include "TimeoutItem.h"
#include "CaptureContext.h"
#include "getMonotonicTimeval.h"
#include "compareTimeval.h"
#include "startTimer.h"
#include "emergencyStop.h"

#include "timerCallback.h"
void timerCallback(evutil_socket_t socket, short what, void *arg) {
	struct CaptureContext *context = (struct CaptureContext *) arg;
	if (what & EV_TIMEOUT) {
		pthread_mutex_lock(&context->timeout_mutex);
		struct timeval now;
		struct TimeoutItem *item;
		while ((item = logdelheap_peek(context->timeout_queue)),
			getMonotonicTimeval(&now),
			((item != NULL) && (compareTimeval(&item->expiration, &now) <= 0))) {
			if (logdelheap_poll(&context->timeout_queue, (void **) &item)) emergencyStop(context);
			pthread_mutex_lock(item->mutex);
			pthread_mutex_unlock(&context->timeout_mutex);
			item->callback(item->arg);
			pthread_mutex_unlock(item->mutex);
			free(item);
			pthread_mutex_lock(&context->timeout_mutex);
		};
		startTimer(context);
		pthread_mutex_unlock(&context->timeout_mutex);
	};
};

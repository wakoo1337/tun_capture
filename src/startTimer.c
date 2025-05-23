#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <event2/event.h>
#include "contrib/logdel_heap.h"
#include "CaptureContext.h"
#include "TimeoutItem.h"
#include "getMonotonicTimeval.h"
#include "subTimeval.h"

#include "startTimer.h"
unsigned int startTimer(struct CaptureContext *context) {
	struct TimeoutItem *item;
	item = logdelheap_peek(context->timeout_queue);
	if (item != NULL) {
		struct timeval tv, delta;
		getMonotonicTimeval(&tv);
		subTimeval(&item->expiration, &tv, &delta);
		if (delta.tv_sec < 0) {
			delta.tv_sec = 0;
			delta.tv_usec = 0;
		};
		return (unsigned int) event_add(context->timeout_event, &delta);
	} else return (unsigned int) event_del(context->timeout_event);
};

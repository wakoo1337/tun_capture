#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <event2/event.h>
#include "contrib/heap.h"
#include "TimeoutItem.h"
#include "CaptureContext.h"
#include "getMonotonicTimeval.h"
#include "subTimeval.h"

#include "startTimer.h"
unsigned int startTimer(struct CaptureContext *context) {
	struct TimeoutItem *item;
	item = heap_peek(context->timeout_queue);
	if (item != NULL) {
		struct timeval tv, delta;
		getMonotonicTimeval(&tv);
		subTimeval(&item->expiration, &tv, &delta);
		event_add(context->timeout_event, &delta);
	};
	return 0;
};

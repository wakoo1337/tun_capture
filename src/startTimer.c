#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/time.h>
#include <event2/event.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "TimeoutItem.h"
#include "CaptureContext.h"
#include "getMonotonicTimeval.h"
#include "subTimeval.h"

#include "startTimer.h"
unsigned int startTimer(struct CaptureContext *context) {
	struct TimeoutItem *item;
	pq_status_t status;
	item = pq_inspect_next(context->timeout_queue, &status);
	if ((item != NULL) && (status == PQ_STATUS_SUCCESS)) {
		struct timeval tv, delta;
		getMonotonicTimeval(&tv);
		subTimeval(&item->expiration, &tv, &delta);
		event_add(context->timeout_event, &delta);
		return 0;
	} else if (status != PQ_STATUS_QUEUE_EMPTY) {
		// TODO аварийное завершение
		return 1;
	};
	return 0;
};

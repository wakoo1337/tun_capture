#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <event2/event.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "TimeoutItem.h"
#include "CaptureContext.h"
#include "cleanDeletedTimeoutItems.h"
#include "subTimeval.h"
#include "getMonotonicTimeval.h"
#include "compareTimeval.h"

#include "timerCallback.h"
void timerCallback(evutil_socket_t socket, short what, void *arg) {
	struct CaptureContext *context = (struct CaptureContext *) arg;
	if (what & EV_TIMEOUT) {
		pthread_mutex_lock(&context->timeout_mutex);
		struct timeval tv;
		cleanDeletedTimeoutItems(context);
		getMonotonicTimeval(&tv);
		struct TimeoutItem *item;
		pq_status_t status;
		while ((item = pq_inspect_next(context->timeout_queue, &status)),
			((item != NULL) && (status == PQ_STATUS_SUCCESS) && (!item->is_del) && (compareTimeval(&item->expiration, &tv) <= 0))) {
			item->callback(item->arg);
			free(item);
		};
		if ((status != PQ_STATUS_SUCCESS) && (status != PQ_STATUS_QUEUE_EMPTY)) {
			// TODO аварийное завершение
		};
		item = pq_inspect_next(context->timeout_queue, &status);
		if ((item != NULL) && (status == PQ_STATUS_SUCCESS)) {
			getMonotonicTimeval(&tv);
			struct timeval delta;
			subTimeval(&item->expiration, &tv, &delta);
			event_add(context->timeout_event, &delta);
		} else if (status != PQ_STATUS_QUEUE_EMPTY) {
			// TODO аварийное завершение
		};
		pthread_mutex_unlock(&context->timeout_mutex);
	};
};

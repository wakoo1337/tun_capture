#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <event2/event.h>
#include "contrib/C-Collections/pqlib/PQ.h"
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
		pq_status_t status;
		while ((item = pq_inspect_next(context->timeout_queue, &status)),
			((item != NULL) && (status == PQ_STATUS_SUCCESS) && (item->is_del || (compareTimeval(&item->expiration, &tv) <= 0)))) {
			item = pq_dequeue(context->timeout_queue, &status);
			if (status != PQ_STATUS_SUCCESS) {
				// TODO аварийное завершение
			};
			if (!item->is_del) item->callback(item->arg);
			free(item);	
		};
		if ((status != PQ_STATUS_SUCCESS) && (status != PQ_STATUS_QUEUE_EMPTY)) {
			// TODO аварийное завершение
		};
		startTimer(context);
		pthread_mutex_unlock(&context->timeout_mutex);
	};
};

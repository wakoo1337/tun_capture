#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "TimeoutItem.h"
#include "CaptureContext.h"

#include "cleanDeletedTimeoutItems.h"
void cleanDeletedTimeoutItems(struct CaptureContext *context) {
	struct TimeoutItem *first;
	pq_status_t status;
	while (pq_inspect_next(context->timeout_queue, &status),
		((status == PQ_STATUS_SUCCESS) && (first != NULL) && (first->is_del))) {
		first = pq_dequeue(context->timeout_queue, &status);
		if (status == PQ_STATUS_SUCCESS) {
			free(first);
		} else {
			// TODO аварийное завершение
		};
	};
};

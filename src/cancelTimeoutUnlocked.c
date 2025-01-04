#include <pthread.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include "contrib/logdel_heap.h"
#include "CaptureContext.h"
#include "TimeoutItem.h"

#include "cancelTimeoutUnlocked.h"
unsigned int cancelTimeoutUnlocked(struct CaptureContext *context, struct TimeoutItem **item) {
	if (*item) {
		void *out;
		if (logdelheap_delete(&context->timeout_queue, &out, (*item)->index)) return 1;
		assert(out == ((void *) (*item)));
		free(*item);
		*item = NULL;
	};
	return 0;
};

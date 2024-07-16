#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "TimeoutItem.h"

#include "cancelTimeout.h"
void cancelTimeout(struct CaptureContext *context, struct TimeoutItem **item) {
	pthread_mutex_lock(&context->timeout_mutex);
	if (NULL != (*item)) {
		(*item)->is_del = true;
		*item = NULL;
	};
	pthread_mutex_unlock(&context->timeout_mutex);
};

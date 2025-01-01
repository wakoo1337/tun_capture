#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include "contrib/logdel_heap.h"
#include "CaptureContext.h"
#include "TimeoutItem.h"

#include "cancelTimeout.h"
unsigned int cancelTimeout(struct CaptureContext *context, pthread_mutex_t *mutex, struct TimeoutItem *item) {
	pthread_mutex_unlock(mutex);
	pthread_mutex_lock(&context->timeout_mutex);
	pthread_mutex_lock(mutex);
	void *out;
	if (logdelheap_delete(&context->timeout_queue, &out, item->index)) return 1;
	assert(out == ((void *) item));
	pthread_mutex_unlock(&context->timeout_mutex);
	return 0;
};

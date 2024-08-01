#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "TimeoutItem.h"

#include "enqueueTimeout.h"
struct TimeoutItem *enqueueTimeout(struct CaptureContext *context, const struct timeval *timeout, void (*callback)(void *), void *arg, pthread_mutex_t *mutex) {
	struct TimeoutItem *item;
	item = malloc(sizeof(struct TimeoutItem));
	if (NULL == item) return NULL;
	item->expiration = *timeout;
	item->callback = callback;
	item->arg = arg;
	item->mutex = mutex;
	item->is_del = false;
	int result;
	result = heap_offer(&context->timeout_queue, item);
	if (result) {
		free(item);
		return NULL;
	};
	return item;
};

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "TimeoutItem.h"

#include "enqueueTimeout.h"
struct TimeoutItem *enqueueTimeout(struct CaptureContext *context, const struct timeval *timeout, void (*callback)(void *), void *arg) {
	struct TimeoutItem *item;
	item = malloc(sizeof(struct TimeoutItem));
	if (NULL == item) return NULL;
	item->expiration = *timeout;
	item->callback = callback;
	item->arg = arg;
	item->is_del = false;
	int result;
	pthread_mutex_lock(&context->timeout_mutex);
	result = heap_offer(&context->timeout_queue, item);
	pthread_mutex_unlock(&context->timeout_mutex);
	if (result) {
		free(item);
		return NULL;
	};
	return item;
};

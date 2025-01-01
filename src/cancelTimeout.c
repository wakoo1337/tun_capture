#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include "contrib/logdel_heap.h"
#include "CaptureContext.h"
#include "TimeoutItem.h"
#include "cancelTimeoutUnlocked.h"

#include "cancelTimeout.h"
unsigned int cancelTimeout(struct CaptureContext *context, pthread_mutex_t *mutex, struct TimeoutItem *item) {
	pthread_mutex_unlock(mutex);
	pthread_mutex_lock(&context->timeout_mutex);
	pthread_mutex_lock(mutex);
	unsigned int result;
	result = cancelTimeoutUnlocked(context, item);
	pthread_mutex_unlock(&context->timeout_mutex);
	return result;
};

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "TimeoutItem.h"
#include "cancelTimeoutUnlocked.h"

#include "cancelTimeout.h"
void cancelTimeout(struct CaptureContext *context, pthread_mutex_t *mutex, struct TimeoutItem **item) {
	pthread_mutex_unlock(mutex);
	pthread_mutex_lock(&context->timeout_mutex);
	pthread_mutex_lock(mutex);
	cancelTimeoutUnlocked(item);
	pthread_mutex_unlock(&context->timeout_mutex);
};

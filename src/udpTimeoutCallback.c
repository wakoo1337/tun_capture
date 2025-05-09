#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "contrib/logdel_heap.h"
#include "CaptureContext.h"
#include "UDPBinding.h"
#include "TimeoutItem.h"
#include "udpFinalizeRead.h"
#include "udpFinalizeWrite.h"

#include "udpTimeoutCallback.h"
bool udpTimeoutCallback(struct CaptureContext *context, struct TimeoutItem *timeout_item) {
	struct UDPBinding *binding = (struct UDPBinding *) timeout_item->arg;
	void *deleted;
	if (logdelheap_delete(&context->timeout_queue, &deleted, timeout_item->index)) {
		pthread_mutex_unlock(&context->timeout_mutex);
		return true;
	};
	assert(deleted == timeout_item);
	pthread_mutex_lock(&binding->mutex);
	udpFinalizeRead(binding);
	udpFinalizeWrite(binding);
	pthread_mutex_unlock(&binding->mutex);
	pthread_mutex_unlock(&context->timeout_mutex);
	return false;
};

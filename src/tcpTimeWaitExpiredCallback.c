#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "contrib/logdel_heap.h"
#include "CaptureContext.h"
#include "TimeoutItem.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "tcpFinalizeRead.h"
#include "tcpFinalizeWrite.h"

#include "tcpTimeWaitExpiredCallback.h"
bool tcpTimeWaitExpiredCallback(struct CaptureContext *context, struct TimeoutItem *timeout_item) {
	struct TCPConnection *connection = (struct TCPConnection *) timeout_item->arg;
	pthread_mutex_lock(&connection->mutex);
	assert(connection->context == context);
	assert(timeout_item == connection->timewait_item);
	void *deleted;
	if (logdelheap_delete(&context->timeout_queue, &deleted, timeout_item->index)) {
		pthread_mutex_unlock(&context->timeout_mutex);
		pthread_mutex_unlock(&connection->mutex);
		return true;
	};
	assert(deleted == timeout_item);
	free(timeout_item);
	connection->timewait_item = NULL;
	pthread_mutex_unlock(&context->timeout_mutex);
	tcpFinalizeRead(connection); // Вообще, если соединение в состоянии TimeWait, то событие чтения уже уничтожено
	tcpFinalizeWrite(connection);
	pthread_mutex_unlock(&connection->mutex);
	return false;
};

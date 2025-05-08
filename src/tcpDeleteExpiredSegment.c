#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "contrib/avl.h"
#include "contrib/logdel_heap.h"
#include "CaptureContext.h"
#include "TimeoutItem.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPSitePrequeueItem.h"

#include "tcpDeleteExpiredSegment.h"
bool tcpDeleteExpiredSegment(struct CaptureContext *context, struct TimeoutItem *timeout_item) {
	struct TCPSitePrequeueItem *item = (struct TCPSitePrequeueItem *) timeout_item->arg;
	struct TCPConnection *connection = item->connection;
	void *deleted;
	if (logdelheap_delete(&context->timeout_queue, &deleted, timeout_item->index)) {
		pthread_mutex_unlock(&context->timeout_mutex);
		return true;
	};
	assert(deleted == timeout_item);
	pthread_mutex_lock(&connection->mutex);
	free(timeout_item);
	item->timeout = NULL;
	pthread_mutex_unlock(&context->timeout_mutex);
	if (item->deleteable) {
		deleted = avl_delete(connection->site_prequeue, item);
		assert(deleted == item);
		free(item->free_me);
		free(item);
	};
	pthread_mutex_unlock(&connection->mutex);
	return false;
};

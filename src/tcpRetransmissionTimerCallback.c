#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "contrib/logdel_heap.h"
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TimeoutItem.h"
#include "TCPAppQueueItem.h"
#include "enqueueTCPPacketTransmission.h"
#include "incrementAppQueueItemRefCount.h"
#include "decrementAppQueueItemRefCount.h"
#include "freeNoRefsAppQueueItem.h"
#include "getMonotonicTimeval.h"
#include "addTimeval.h"
#include "retry_delay.h"

#include "tcpRetransmissionTimerCallback.h"
bool tcpRetransmissionTimerCallback(struct CaptureContext *context, struct TimeoutItem *timeout_item) {
	struct TCPAppQueueItem *item;
	item = (struct TCPAppQueueItem *) timeout_item->arg;
	struct TCPConnection *connection;
	connection = item->connection;
	pthread_mutex_lock(&connection->mutex);
	incrementAppQueueItemRefCount(item);
	enqueueTCPPacketTransmission(item);
	struct timeval now;
	getMonotonicTimeval(&now);
	addTimeval(&now, &retry_delay, &timeout_item->expiration);
	if (logdelheap_modify_key(context->timeout_queue, timeout_item, timeout_item->index)) {
		pthread_mutex_unlock(&context->timeout_mutex);
		decrementAppQueueItemRefCount(item);
		freeNoRefsAppQueueItem(item);
		pthread_mutex_unlock(&connection->mutex);
		return true;
	};
	pthread_mutex_unlock(&context->timeout_mutex);
	decrementAppQueueItemRefCount(item);
	freeNoRefsAppQueueItem(item);
	pthread_mutex_unlock(&connection->mutex);
	return false;
};

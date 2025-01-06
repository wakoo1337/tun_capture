#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include "contrib/logdel_heap.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"
#include "freeNoRefsAppQueueItem.h"
#include "decrementAppQueueItemRefCount.h"

#include "sendTCPPacketRefcounted.h"
unsigned int sendTCPPacketRefcounted(struct CaptureContext *context, uint8_t *packet, unsigned int size, void *arg) {
	struct TCPAppQueueItem *item;
	item = (struct TCPAppQueueItem *) arg;
	assert(context == item->connection->context);
	ssize_t result;
	result = context->settings->write_function(packet, size, context->settings->user);
	int old_errno = errno;
	pthread_mutex_t *mutex = &item->connection->mutex;
	pthread_mutex_lock(mutex);
	decrementAppQueueItemRefCount(item);
	freeNoRefsAppQueueItem(item);
	pthread_mutex_unlock(mutex);
	return (result == -1) && ((old_errno != EAGAIN) && (old_errno != EWOULDBLOCK));
};

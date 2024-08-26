#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include "contrib/heap.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"
#include "cancelTimeout.h"

#include "sendTCPPacketRefcounted.h"
unsigned int sendTCPPacketRefcounted(struct CaptureContext *context, uint8_t *packet, unsigned int size, void *arg) {
	struct TCPAppQueueItem *item;
	item = (struct TCPAppQueueItem *) arg;
	ssize_t result;
	result = context->settings->write_function(packet, size, context->settings->user);
	pthread_mutex_lock(&item->connection->mutex);
	if (-1 == result) {
		item->ref_count--;
		pthread_mutex_unlock(&item->connection->mutex);
		return (errno != EAGAIN) && (errno != EWOULDBLOCK);
	};
	item->ref_count--;
	if (0 == item->ref_count) {
		assert(context == item->connection->context);
		cancelTimeout(context, &item->connection->mutex, &item->timeout);
		pthread_mutex_t *mutex = &item->connection->mutex;
		free(item->free_me);
		free(item);
		pthread_mutex_unlock(mutex);
	} else pthread_mutex_unlock(&item->connection->mutex);
	return 0;
};

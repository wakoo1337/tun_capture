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
#include "cancelTimeoutUnlocked.h"

#include "sendTCPPacketRefcounted.h"
unsigned int sendTCPPacketRefcounted(struct CaptureContext *context, uint8_t *packet, unsigned int size, void *arg) {
	struct TCPAppQueueItem *item;
	item = (struct TCPAppQueueItem *) arg;
	assert(context == item->connection->context);
	ssize_t result;
	result = context->settings->write_function(packet, size, context->settings->user);
	pthread_mutex_t *mutex = &item->connection->mutex;
	pthread_mutex_lock(&context->timeout_mutex);
	pthread_mutex_lock(mutex);
	item->ref_count--;
	if (0 == item->ref_count) {
		cancelTimeoutUnlocked(&item->timeout);
		free(item->free_me);
		free(item);
	};
	pthread_mutex_unlock(mutex);
	pthread_mutex_unlock(&context->timeout_mutex);
	return (result == -1) && ((errno != EAGAIN) && (errno != EWOULDBLOCK));
};

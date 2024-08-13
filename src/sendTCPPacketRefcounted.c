#include <pthread.h>
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

#include "sendTCPPacketRefcounted.h"
unsigned int sendTCPPacketRefcounted(struct CaptureContext *context, uint8_t *packet, unsigned int size, void *arg) {
	struct TCPAppQueueItem *item;
	item = (struct TCPAppQueueItem *) arg;
	ssize_t result;
	result = context->settings->write_function(packet, size, context->settings->user);
	if (-1 == result) return (errno != EAGAIN) && (errno != EWOULDBLOCK);
	pthread_mutex_lock(&item->connection->mutex);
	item->ref_count--;
	if (0 == item->ref_count) {
		free(item->free_me);
		pthread_mutex_unlock(&item->connection->mutex);
		free(item);
	} else pthread_mutex_unlock(&item->connection->mutex);
	return 0;
};

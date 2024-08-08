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
#include "PacketQueueItem.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"

#include "sendTCPPacketRefcounted.h"
unsigned int sendTCPPacketRefcounted(struct CaptureContext *context, uint8_t *packet, unsigned int size, void *arg) {
	struct PacketQueueItem *item;
	item = (struct PacketQueueItem *) arg;
	struct TCPAppQueueItem *segment;
	segment = (struct TCPAppQueueItem *) item->arg;
	ssize_t result;
	result = context->settings->write_function(packet, size, context->settings->user);
	if (-1 == result) return (errno != EAGAIN) && (errno != EWOULDBLOCK);
	pthread_mutex_lock(&segment->connection->mutex);
	segment->ref_count--;
	if (0 == segment->ref_count) {
		free(segment->free_me);
		free(segment);
	};
	pthread_mutex_unlock(&segment->connection->mutex);
	return 0;
};

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "RefcountBuffer.h"
#include "PacketQueueItem.h"
#include "dequeueRxPacket.h"
#include "emergencyStop.h"
#include "decrementRefcount.h"

#include "threadWorker.h"
void *threadWorker(void *arg) {
	struct CaptureContext *context = (struct CaptureContext *) arg;
	struct PacketQueueItem *packet;
	while (context->running) {
		dequeueRxPacket(context, &packet);
		pthread_mutex_lock(&packet->buffer->mutex);
		if (packet->buffer->size == 0) {
			if (packet->mutex) pthread_mutex_unlock(packet->mutex);
			decrementRefcount(&packet->buffer);
			free(packet);
			continue;
		};
		if (packet->processor(context, packet->buffer, packet->arg)) {
			emergencyStop(context);
			if (packet->mutex) pthread_mutex_unlock(packet->mutex);
			free(packet);
			return NULL;
		};
		if (packet->mutex) pthread_mutex_unlock(packet->mutex);
		free(packet);
	};
	return NULL;
};

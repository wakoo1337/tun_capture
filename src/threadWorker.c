#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "dequeueRxPacket.h"
#include "emergencyStop.h"

#include "threadWorker.h"
void *threadWorker(void *arg) {
	struct CaptureContext *context = (struct CaptureContext *) arg;
	struct PacketQueueItem *packet;
	while (context->running) {
		dequeueRxPacket(context, &packet);
		if (packet->processor(context, packet->data, packet->count, packet->arg)) {
			emergencyStop(context);
			if (packet->mutex) pthread_mutex_unlock(packet->mutex);
			free(packet);
			return NULL;
		};
		if (packet->mutex) pthread_mutex_unlock(packet->mutex);
		if (packet->semaphore) sem_post(packet->semaphore);
		free(packet);
	};
	return NULL;
};

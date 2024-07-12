#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "dequeuePacket.h"

#include "threadWorker.h"
void *threadWorker(void *arg) {
	struct CaptureContext *context = (struct CaptureContext *) arg;
	struct PacketQueueItem *packet;
	while (true) {
		dequeuePacket(context, &packet);
		if (packet->count == 0) {
			free(packet->free_me);
			free(packet);
		};
		if (packet->processor(context, packet->data, packet->count, packet->arg)) free(packet->free_me);
		free(packet);
	};
};

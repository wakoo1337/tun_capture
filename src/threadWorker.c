#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "dequeuePacket.h"

#include "threadWorker.h"
void *threadWorker(void *arg) {
	struct CaptureContext *context = (struct CaptureContext *) arg;
	struct PacketQueueItem *packet;
	while (true) {
		dequeuePacket(context, &packet);
		free(packet->data);
		free(packet);
	};
};

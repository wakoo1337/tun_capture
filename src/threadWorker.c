#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include "contrib/C-Collections/pqlib/PQ.h"
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
			free(packet->data);
			free(packet);
		};
		if (packet->processor(context, packet->data, packet->count, packet->arg)) free(packet->data);
		free(packet);
	};
};

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "dequeuePacket.h"
#include "processIPv4Packet.h"

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
		uint8_t type = (((uint8_t *) packet->data)[0]) >> 4;
		if (type == 4) {
			if (processIPv4Packet(context, packet->data, packet->count)) free(packet->data);
			free(packet);
		} else if (type == 6) {
			free(packet->data);
			free(packet);
		} else {
			free(packet->data);
			free(packet);
		};
	};
};

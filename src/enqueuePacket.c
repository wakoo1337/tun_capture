#include <pthread.h>
#include <stdint.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"

#include "enqueuePacket.h"
void enqueuePacket(struct CaptureContext *context, struct PacketQueueItem *item) {
	item->next = NULL;
	*(context->captured_end) = item;
	context->captured_end = &item->next;
};

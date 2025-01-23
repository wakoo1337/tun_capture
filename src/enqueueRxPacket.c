#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <semaphore.h>
#include "contrib/logdel_heap.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"

#include "enqueueRxPacket.h"
void enqueueRxPacket(struct CaptureContext *context, struct PacketQueueItem *item) {
	item->next = NULL;
	pthread_mutex_lock(&context->rx_mutex);
	*(context->rx_end) = item;
	context->rx_end = &item->next;
	pthread_cond_signal(&context->rx_cond);
	pthread_mutex_unlock(&context->rx_mutex);
};

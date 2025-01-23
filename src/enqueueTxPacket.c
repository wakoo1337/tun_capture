#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <event2/event.h>
#include "contrib/logdel_heap.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"

#include "enqueueTxPacket.h"
unsigned int enqueueTxPacket(struct CaptureContext *context, struct PacketQueueItem *item) {
	item->next = NULL;
	pthread_mutex_lock(&context->tx_mutex);
	*(context->tx_end) = item;
	context->tx_end = &item->next;
	int result;
	result = event_add(context->tx_event, NULL);
	pthread_mutex_unlock(&context->tx_mutex);
	return (unsigned int) result;
};

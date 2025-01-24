#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include "CaptureContext.h"
#include "PacketQueueItem.h"

#include "dequeueTxPacket.h"
struct PacketQueueItem *dequeueTxPacket(struct CaptureContext *context, struct PacketQueueItem **item) {
	pthread_mutex_lock(&context->tx_mutex);
	*item = context->tx_begin;
	if (context->tx_begin) context->tx_begin = context->tx_begin->next;
	if (NULL == context->tx_begin) context->tx_end = &context->tx_begin;
	pthread_mutex_unlock(&context->tx_mutex);
	return *item;
};

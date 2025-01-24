#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include "CaptureContext.h"
#include "PacketQueueItem.h"

#include "dequeueRxPacket.h"
unsigned int dequeueRxPacket(struct CaptureContext *context, struct PacketQueueItem **item) {
	pthread_mutex_lock(&context->rx_mutex);
	while (true) {
		if (context->rx_begin) {
			*item = context->rx_begin;
			context->rx_begin = context->rx_begin->next;
			if (NULL == context->rx_begin) context->rx_end = &context->rx_begin;
			if ((*item)->semaphore != NULL) sem_wait((*item)->semaphore);
			if ((*item)->mutex != NULL) pthread_mutex_lock((*item)->mutex);
			pthread_mutex_unlock(&context->rx_mutex);
			return 0;
		} else {
			context->rx_end = &context->rx_begin;
			pthread_cond_wait(&context->rx_cond, &context->rx_mutex);
		};
	};
	return 1; // Если исполнение вываливается из бесконечного цикла, это точно ошибка
};

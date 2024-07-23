#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"

#include "dequeuePacket.h"
unsigned int dequeuePacket(struct CaptureContext *context, struct PacketQueueItem **item) {
	pthread_mutex_lock(&context->queue_mutex);
	while (true) {
		if (context->captured_begin) {
			*item = context->captured_begin;
			context->captured_begin = context->captured_begin->next;
			if (NULL == context->captured_begin) context->captured_end = &context->captured_begin;
			if ((*item)->mutex != NULL) pthread_mutex_lock((*item)->mutex);
			pthread_mutex_unlock(&context->queue_mutex);
			return 0;
		} else {
			context->captured_end = &context->captured_begin;
			pthread_cond_wait(&context->queue_cond, &context->queue_mutex);
		};
	};
	return 1; // Если исполнение вываливается из бесконечного цикла, это точно ошибка
};

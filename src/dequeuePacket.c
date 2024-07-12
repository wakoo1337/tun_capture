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
		if (context->captured_stack) {
			*item = context->captured_stack;
			context->captured_stack = context->captured_stack->next;
			pthread_mutex_unlock(&context->queue_mutex);
			return 0;
		};
		pthread_cond_wait(&context->queue_cond, &context->queue_mutex);
	};
	return 1; // Если исполнение вываливается из бесконечного цикла, это точно ошибка
};

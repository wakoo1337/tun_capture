#include <pthread.h>
#include <stdlib.h>
#include "RefcountBuffer.h"

#include "createRefcountBuffer.h"
struct RefcountBuffer *createRefcountBuffer(unsigned int capacity) {
	struct RefcountBuffer *buffer;
	buffer = malloc(sizeof(struct RefcountBuffer));
	if (NULL == buffer) return NULL;
	buffer->data = malloc(capacity);
	if (NULL == buffer->data) {
		free(buffer);
		return NULL;
	};
	pthread_mutex_init(&buffer->mutex, NULL);
	pthread_mutex_lock(&buffer->mutex);
	buffer->ref_count = 1;
	pthread_mutex_unlock(&buffer->mutex);
	buffer->capacity = capacity;
	buffer->netpkt_offset = buffer->trapkt_offset = buffer->payload_offset = 0;
	return buffer;
};

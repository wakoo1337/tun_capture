#include <pthread.h>
#include <stdlib.h>
#include "RefcountBuffer.h"

#include "incrementRefcount.h"
void incrementRefcount(struct RefcountBuffer *buffer) {
	pthread_mutex_lock(&buffer->mutex);
	buffer->ref_count++;
	pthread_mutex_unlock(&buffer->mutex);
};

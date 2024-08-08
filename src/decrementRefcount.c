#include <pthread.h>
#include <stdlib.h>
#include "RefcountBuffer.h"

#include "decrementRefcount.h"
void decrementRefcount(struct RefcountBuffer **buffer) {
	unsigned int new_ref;
	pthread_mutex_lock(&((*buffer)->mutex));
	(*buffer)->ref_count--;
	new_ref = (*buffer)->ref_count;
	pthread_mutex_unlock(&((*buffer)->mutex));
	if (new_ref == 0) {
		pthread_mutex_destroy(&((*buffer)->mutex));
		free((*buffer)->data);
		free(*buffer);
		*buffer = NULL;
	};
};

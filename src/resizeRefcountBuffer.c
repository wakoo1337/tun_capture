#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include "RefcountBuffer.h"

#include "resizeRefcountBuffer.h"
void resizeRefcountBuffer(struct RefcountBuffer **buffer, unsigned int new_capacity) {
	uint8_t *old = (*buffer)->data;
	assert((*buffer)->ref_count == 1);
	(*buffer)->data = realloc((*buffer)->data, new_capacity);
	if (NULL == (*buffer)->data) {
		free(old);
		free(*buffer);
		*buffer = NULL;
		return;
	};
	(*buffer)->size = new_capacity;
	(*buffer)->packet_offset = (((*buffer)->packet_offset) >= new_capacity) ? (new_capacity - 1) : ((*buffer)->packet_offset);
	(*buffer)->payload_offset = (((*buffer)->payload_offset) >= new_capacity) ? (new_capacity - 1) : ((*buffer)->payload_offset);
};

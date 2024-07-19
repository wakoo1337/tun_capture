#include <pthread.h>
#include <stdint.h>
#include "contrib/heap.h"
#include "CaptureContext.h"

#include "processTCPData.h"
unsigned int processTCPData(struct CaptureContext *context, uint8_t *packet, unsigned int count, void *arg) {
	return 1;
};

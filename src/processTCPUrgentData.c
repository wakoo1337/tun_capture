#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include "contrib/logdel_heap.h"
#include "CaptureContext.h"

#include "processTCPUrgentData.h"
unsigned int processTCPUrgentData(struct CaptureContext *context, uint8_t *packet, unsigned int count, void *arg) {
	return 1;
};

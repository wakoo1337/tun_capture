#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "CaptureContext.h"
#include "HEADERS_RESERVE.h"

#include "udpProcessor.h"
unsigned int udpProcessor(struct CaptureContext *context, uint8_t *packet, unsigned int size, void *arg) {
	free(&packet[-HEADERS_RESERVE]);
	free(arg);
	return 0;
};

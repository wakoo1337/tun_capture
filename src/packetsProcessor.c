#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "RefcountBuffer.h"
#include "processIPv4Packet.h"
#include "decrementRefcount.h"

#include "packetsProcessor.h"
unsigned int packetsProcessor(struct CaptureContext *context, struct RefcountBuffer *buffer, void *arg) {
	uint8_t type;
	type = buffer->data[buffer->netpkt_offset];
	if (type == 4) return processIPv4Packet(context, buffer);
	else if (type == 6) decrementRefcount(&buffer);
	else decrementRefcount(&buffer);
	return 0;
};

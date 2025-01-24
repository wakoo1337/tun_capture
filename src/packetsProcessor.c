#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "CaptureContext.h"
#include "processIPv4Packet.h"

#include "packetsProcessor.h"
unsigned int packetsProcessor(struct CaptureContext *context, uint8_t *packet, unsigned int size, void *arg) {
	uint8_t type = packet[0] >> 4;
	if (type == 4) return processIPv4Packet(context, packet, size);
	else if (type == 6) free(packet);
	else free(packet);
	return 0;
};

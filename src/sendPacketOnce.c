#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "contrib/heap.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"

#include "sendPacketOnce.h"
unsigned int sendPacketOnce(struct CaptureContext *context, uint8_t *packet, unsigned int size, void *arg) {
	ssize_t result;
	result = context->settings->write_function(packet, size, context->settings->user);
	if (-1 == result) {
		return (errno != EAGAIN) && (errno != EWOULDBLOCK);
	};
	struct PacketQueueItem *item;
	item = (struct PacketQueueItem *) arg;
	free(item->free_me);
	free(item);
	return 0;
};

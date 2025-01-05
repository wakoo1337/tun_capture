#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"
#include "cancelTimeout.h"

#include "freeNoRefsAppQueueItem.h"
void freeNoRefsAppQueueItem(struct TCPAppQueueItem *item) {
	if (0 == item->ref_count) {
		cancelTimeout(item->connection->context, &item->connection->mutex, &item->timeout);
		free(item->free_me);
		free(item);
	};
};

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"
#include "decrementAppQueueItemRefCount.h"
#include "cancelTimeoutUnlocked.h"

#include "tcpDestroyAppPrequeueItem.h"
void tcpDestroyAppPrequeueItem(void *item_void, void *) {
	struct TCPAppQueueItem *item = (struct TCPAppQueueItem *) item_void;
	decrementAppQueueItemRefCount(item);
	if (0 == item->ref_count) {
		cancelTimeoutUnlocked(item->connection->context, &item->timeout);
		free(item->free_me);
		free(item);
	};
};

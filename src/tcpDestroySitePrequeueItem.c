#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "TCPSitePrequeueItem.h"
#include "cancelTimeoutUnlocked.h"

#include "tcpDestroySitePrequeueItem.h"
void tcpDestroySitePrequeueItem(void *item_void, void *) {
	struct TCPSitePrequeueItem *item = (struct TCPSitePrequeueItem *) item_void;
	if (item->deleteable) {
		cancelTimeoutUnlocked(item->connection->context, &item->timeout);
		free(item->free_me);
		free(item);
	};
};

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
void tcpDestroySitePrequeueItem(void *item_void, void *futile) {
	struct TCPSitePrequeueItem *item = (struct TCPSitePrequeueItem *) item_void;
	pthread_mutex_t *timeout_mutex = &item->connection->context->timeout_mutex;
	pthread_mutex_unlock(&item->connection->mutex);
	pthread_mutex_lock(timeout_mutex);
	pthread_mutex_lock(&item->connection->mutex);
	cancelTimeoutUnlocked(item->connection->context, &item->timeout);
	pthread_mutex_unlock(timeout_mutex);
	free(item->free_me);
	free(item);
};

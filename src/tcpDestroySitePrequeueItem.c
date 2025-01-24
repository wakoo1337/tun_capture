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
#include "cancelTimeout.h"

#include "tcpDestroySitePrequeueItem.h"
void tcpDestroySitePrequeueItem(void *item_void, void *) {
	// Вызывать либо из главного потока, либо когда предочередь пуста, и функция не выполнится
	struct TCPSitePrequeueItem *item = (struct TCPSitePrequeueItem *) item_void;
	cancelTimeout(item->connection->context, &item->connection->mutex, &item->timeout);
	free(item->free_me);
	free(item);
};

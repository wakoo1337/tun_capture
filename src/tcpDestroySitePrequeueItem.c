#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPSitePrequeueItem.h"
#include "cancelTimeout.h"

#include "tcpDestroySitePrequeueItem.h"
void tcpDestroySitePrequeueItem(void *item_void, void *futile) {
	struct TCPSitePrequeueItem *item = (struct TCPSitePrequeueItem *) item_void;
	cancelTimeout(item->connection->context, &item->connection->mutex, item->timeout);
	free(item->free_me);
	free(item);
};

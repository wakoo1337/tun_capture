#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "contrib/avl.h"
#include "TimeoutItem.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPSitePrequeueItem.h"

#include "tcpDeleteExpiredSegment.h"
void tcpDeleteExpiredSegment(void *arg) {
	struct TCPSitePrequeueItem *item = (struct TCPSitePrequeueItem *) arg;
	void *deleted;
	deleted = avl_delete(item->connection->site_prequeue, item);
	assert(deleted == arg);
	free(item->free_me);
	free(item);
};

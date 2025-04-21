#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "contrib/avl.h"
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "TCPSitePrequeueItem.h"
#include "TimeoutItem.h"
#include "cancelTimeout.h"
#include "prequeueItemToSiteData.h"

#include "prequeueToSiteQueue.h"
unsigned int prequeueToSiteQueue(struct TCPConnection *connection, unsigned int (*on_fin)(struct TCPConnection *)) {
	struct TCPSitePrequeueItem *found_prequeue;
	while ((found_prequeue = avl_find(connection->site_prequeue, &connection->first_desired))) {
		found_prequeue->deleteable = false;
		void *deleted;
		deleted = avl_delete(connection->site_prequeue, found_prequeue);
		assert(deleted == found_prequeue);
		cancelTimeout(connection->context, &connection->mutex, &found_prequeue->timeout);
		prequeueItemToSiteData(connection, found_prequeue);
		if (found_prequeue->fin && on_fin(connection)) {
			free(found_prequeue);
			return 1;
		};
		free(found_prequeue);
	};
	return 0;
};

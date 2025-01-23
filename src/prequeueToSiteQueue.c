#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "contrib/avl.h"
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "TCPSitePrequeueItem.h"
#include "TimeoutItem.h"
#include "cancelTimeoutUnlocked.h"
#include "prequeueItemToSiteData.h"

#include "prequeueToSiteQueue.h"
unsigned int prequeueToSiteQueue(struct TCPConnection *connection, unsigned int (*on_fin)(struct TCPConnection *)) {
	struct TCPSitePrequeueItem *found_prequeue;
	while ((found_prequeue = avl_find(connection->site_prequeue, &connection->first_desired))) {
		pthread_mutex_unlock(&connection->mutex);
		pthread_mutex_lock(&connection->context->timeout_mutex);
		pthread_mutex_lock(&connection->mutex);
		if ((found_prequeue = avl_find(connection->site_prequeue, &connection->first_desired))) {
			// За время ожидания на мьютексах таймер может сработать, а элемент предочереди удалиться
			cancelTimeoutUnlocked(connection->context, &found_prequeue->timeout);
			pthread_mutex_unlock(&connection->context->timeout_mutex);
			prequeueItemToSiteData(connection, found_prequeue);
			if (found_prequeue->fin && on_fin(connection)) {
				free(found_prequeue);
				return 1;
			};
			free(found_prequeue);
		} else pthread_mutex_unlock(&connection->context->timeout_mutex);
	};
	return 0;
};

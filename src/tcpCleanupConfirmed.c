#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "contrib/heap.h"
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPAppQueueItem.h"
#include "TCPConnection.h"
#include "cancelTimeout.h"

#include "tcpCleanupConfirmed.h"
void tcpCleanupConfirmed(struct TCPConnection *connection) {
	struct TCPAppQueueItem **found = &connection->app_queue;
	while ((*found) && ((*found)->confirm_ack != connection->latest_ack)) found = &((*found)->next);
	if (*found) {
		struct TCPAppQueueItem *current = connection->app_queue;
		connection->app_queue = (*found)->next;
		if (NULL == connection->app_queue) connection->app_last = &connection->app_queue;
		if (*found) (*found)->next = NULL;
		pthread_mutex_unlock(&connection->mutex);
		while (current) {
			struct TCPAppQueueItem *next = current->next;
			cancelTimeout(connection->context, &current->timeout);
			free(current->free_me);
			free(current);
			current = next;
		};
		pthread_mutex_lock(&connection->mutex);
	};
	if (NULL == connection->app_queue) connection->app_last = &connection->app_queue;
};

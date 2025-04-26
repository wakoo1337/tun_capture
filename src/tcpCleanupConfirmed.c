#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPAppQueueItem.h"
#include "TCPConnection.h"
#include "cancelTimeout.h"
#include "freeNoRefsAppQueueItem.h"
#include "incrementAppQueueItemRefCount.h"
#include "decrementAppQueueItemRefCount.h"

#include "tcpCleanupConfirmed.h"
void tcpCleanupConfirmed(struct TCPConnection *connection) {
	struct TCPAppQueueItem *current = NULL;
	if (connection->latest_ack == connection->seq_first) {
		// Если подтверждены все сегменты, все и удаляем
		current = connection->app_queue;
		connection->app_queue = NULL;
		connection->app_last = &connection->app_queue;
	} else {
		// Если подтверждены не все сегменты, то ищем последний подтверждённый
		struct TCPAppQueueItem *found;
		found = connection->app_queue;
		while (found && found->timeout &&
			(found->confirm_ack != connection->latest_ack)) found = found->next;
		if (found && found->timeout) {
			// Если нашли, удаляем все, что до него
			current = connection->app_queue;
			connection->app_queue = found->next;
			if (NULL == connection->app_queue) connection->app_last = &connection->app_queue;
			found->next = NULL;
		};
	};
	while (current) {
		incrementAppQueueItemRefCount(current);
		const unsigned int new_app_scheduled = connection->app_scheduled - current->data_size;
		connection->app_scheduled = (new_app_scheduled <= connection->app_scheduled) ? new_app_scheduled : 0;
		cancelTimeout(connection->context, &connection->mutex, &current->timeout);
		struct TCPAppQueueItem *next = current->next;
		decrementAppQueueItemRefCount(current);
		decrementAppQueueItemRefCount(current);
		freeNoRefsAppQueueItem(current);
		current = next;
	};
};

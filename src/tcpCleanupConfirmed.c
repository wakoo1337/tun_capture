#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPAppQueueItem.h"
#include "TCPConnection.h"
#include "cancelTimeout.h"

#include "tcpCleanupConfirmed.h"
void tcpCleanupConfirmed(struct TCPConnection *connection) {
	struct TCPAppQueueItem *found = NULL, *current = connection->app_queue;
	while (current) {
		if (current->confirm_ack == connection->latest_ack) found = current;
		current = current->next;
	};
	if (found) {
		struct TCPAppQueueItem *next;
		current = connection->app_queue;
		while ((current != found) && current) {
			next = current->next;
			cancelTimeout(connection->context, &current->timeout);
			free(current->free_me);
			connection->app_scheduled -= current->data_size;
			free(current);
			current = next;
		};
		if (current) {
			next = current->next;
			cancelTimeout(connection->context, &current->timeout);
			free(current->free_me);
			connection->app_scheduled -= current->data_size;
			free(current);
			connection->app_queue = next;
		};
	};
	if (NULL == connection->app_queue) connection->app_last = &connection->app_queue;
};

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPAppQueueItem.h"
#include "TCPConnection.h"

#include "tcpCleanupConfirmed.h"
void tcpCleanupConfirmed(struct TCPConnection *connection, uint32_t ack, unsigned int window) {
	struct TCPAppQueueItem *found = NULL, *current = connection->app_queue;
	if (ack == connection->latest_ack) connection->app_window = window << (connection->scaling_enabled ? connection->remote_scale : 0);
	while (current) {
		if (current->confirm_ack == ack) found = current;
		current = current->next;
	};
	if (found) {
		struct TCPAppQueueItem *next;
		current = connection->app_queue;
		while ((current != found) && current) {
			next = current->next;
			free(current->free_me);
			free(current);
			current = next;
		};
		if (current) {
			next = current->next;
			free(current->free_me);
			free(current);
			connection->app_queue = next;
			connection->latest_ack = ack;
			connection->app_window = window << (connection->scaling_enabled ? connection->remote_scale : 0);
		};
	};
	if (NULL == connection->app_queue) connection->app_last = &connection->app_queue;
};

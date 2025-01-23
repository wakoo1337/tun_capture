#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"

#include "findPreviousNextAppQueueItem.h"
struct TCPAppQueueItem **findPreviousNextAppQueueItem(struct TCPConnection *connection, struct TCPAppQueueItem *search) {
	struct TCPAppQueueItem *current;
	current = connection->app_queue;
	if (current == search) return &connection->app_queue;
	while (current) {
		if (current->next == search) return &current->next;
		current = current->next;
	};
	return NULL;
};

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPSiteQueueItem.h"
#include "TCPConnection.h"

#include "dequeueSiteQueueItem.h"
struct TCPSiteQueueItem *dequeueSiteQueueItem(struct TCPConnection *connection) {
	struct TCPSiteQueueItem *current;
	current = connection->site_queue;
	if (NULL == current) return NULL;
	connection->site_queue = connection->site_queue->next;
	if (NULL == connection->site_queue) connection->site_last = &connection->site_queue;
	return current;
};

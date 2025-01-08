#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include "contrib/avl.h"
#include "contrib/logdel_heap.h"
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"
#include "TCPSiteQueueItem.h"
#include "tcpDestroySitePrequeueItem.h"
#include "cancelTimeout.h"
#include "freeNoRefsAppQueueItem.h"
#include "incrementAppQueueItemRefCount.h"
#include "decrementAppQueueItemRefCount.h"

#include "destroyTCPConnection.h"
void destroyTCPConnection(struct TCPConnection *connection) {
	pthread_mutex_unlock(&connection->mutex); // Избегаем взаимной блокировки
	pthread_mutex_lock(&connection->context->tcp_mutex);
	void *del;
	del = avl_delete(connection->context->tcp_connections, connection);
	assert(del == connection);
	pthread_mutex_lock(&connection->mutex);
	pthread_mutex_unlock(&connection->context->tcp_mutex);
	while (connection->app_queue) {
		incrementAppQueueItemRefCount(connection->app_queue);
		struct TCPAppQueueItem *next;
		next = connection->app_queue->next;
		cancelTimeout(connection->context, &connection->mutex, &connection->app_queue->timeout);
		decrementAppQueueItemRefCount(connection->app_queue);
		decrementAppQueueItemRefCount(connection->app_queue);
		freeNoRefsAppQueueItem(connection->app_queue);
		connection->app_queue = next;
	};
	avl_destroy(connection->site_prequeue, &tcpDestroySitePrequeueItem);
	while (connection->site_queue) {
		struct TCPSiteQueueItem *next;
		next = connection->site_queue->next;
		free(connection->site_queue->free_me);
		free(connection->site_queue);
		connection->site_queue = next;
	};
	pthread_mutex_unlock(&connection->mutex);
	pthread_mutex_destroy(&connection->mutex);
	close(connection->sock);
	free(connection);
}; 

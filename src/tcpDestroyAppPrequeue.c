#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "contrib/avl.h"
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "tcpDestroyAppPrequeueItem.h"

#include "tcpDestroyAppPrequeue.h"
void tcpDestroyAppPrequeue(struct TCPConnection *connection) {
	pthread_mutex_unlock(&connection->mutex);
	pthread_mutex_lock(&connection->context->timeout_mutex);
	pthread_mutex_lock(&connection->mutex);
	avl_destroy(connection->app_prequeue, &tcpDestroyAppPrequeueItem);
	pthread_mutex_unlock(&connection->context->timeout_mutex);
};

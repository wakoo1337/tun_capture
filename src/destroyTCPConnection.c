#include <pthread.h>
#include <assert.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "contrib/avl.h"
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"

#include "destroyTCPConnection.h"
void destroyTCPConnection(struct TCPConnection *connection) {
	pthread_mutex_lock(&connection->context->tcp_mutex);
	pthread_mutex_lock(&connection->mutex);
	void *del;
	del = avl_delete(connection->context->tcp_connections, connection);
	assert(del == connection);
	pthread_mutex_unlock(&connection->context->tcp_mutex);
	pthread_mutex_unlock(&connection->mutex);
	pthread_mutex_destroy(&connection->mutex);
	close(connection->sock);
	free(connection);
}; 

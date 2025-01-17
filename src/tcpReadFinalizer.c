#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <event2/event.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "destroyTCPConnection.h"

#include "tcpReadFinalizer.h"
void tcpReadFinalizer(struct event *fin_event, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	assert(!connection->read_finalized);
	pthread_mutex_lock(&connection->mutex);
	connection->read_finalized = true;
	connection->read_event = NULL;
	if (connection->read_finalized && connection->write_finalized) destroyTCPConnection(connection);
	else pthread_mutex_unlock(&connection->mutex);
};

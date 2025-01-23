#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <event2/event.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "destroyTCPConnection.h"

#include "tcpWriteFinalizer.h"
void tcpWriteFinalizer(struct event *fin_event, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	pthread_mutex_lock(&connection->mutex);
	assert(!connection->write_finalized);
	connection->write_finalized = true;
	connection->write_event = NULL;
	if (connection->read_finalized && connection->write_finalized) destroyTCPConnection(connection);
	else pthread_mutex_unlock(&connection->mutex);
};

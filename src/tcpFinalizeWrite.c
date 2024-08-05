#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <event2/event.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "destroyTCPConnection.h"

#include "tcpFinalizeWrite.h"
void tcpFinalizeWrite(struct event *fin_event, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	pthread_mutex_lock(&connection->mutex);
	connection->write_finalized = true;
	if (connection->read_finalized && connection->write_finalized) destroyTCPConnection(connection);
	else pthread_mutex_unlock(&connection->mutex);
};

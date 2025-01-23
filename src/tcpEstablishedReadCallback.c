#include <pthread.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "readAndEnqueueSiteData.h"
#include "tcpEstablishedOnEnd.h"
#include "tcpEstablishedOnError.h"

#include "tcpEstablishedReadCallback.h"
unsigned int tcpEstablishedReadCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	assert(fd == connection->sock);
	unsigned int result = 0;
	if (what & EV_READ) {
		result = readAndEnqueueSiteData(connection, &tcpEstablishedOnEnd, &tcpEstablishedOnError);
	};
	pthread_mutex_unlock(&connection->mutex);
	return result;
};

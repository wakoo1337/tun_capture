#include <pthread.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "readAndEnqueueSiteData.h"
#include "tcpGotFINOnEnd.h"
#include "tcpGotFINOnError.h"

#include "tcpGotFINReadCallback.h"
unsigned int tcpGotFINReadCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	assert(fd == connection->sock);
	unsigned int result = 0;
	if (what & EV_READ) {
		result = readAndEnqueueSiteData(connection, &tcpGotFINOnEnd, &tcpGotFINOnError);
	};
	pthread_mutex_unlock(&connection->mutex);
	return result;
};

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
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
	if (what & EV_READ) {
		assert(fd == connection->sock);
		readAndEnqueueSiteData(connection, &tcpGotFINOnEnd, &tcpGotFINOnError);
	};
	pthread_mutex_unlock(&connection->mutex);
	return 0;
};

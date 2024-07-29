#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <event2/event.h>
#include <assert.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "destroyTCPConnection.h"
#include "sendSynReply.h"
#include "tcpstate_synack_send.h"

#include "tcpConnwaitWriteCallback.h"
unsigned int tcpConnwaitWriteCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	if (what & EV_WRITE) {
		int err;
		socklen_t sl = sizeof(int);
		if ((-1 == getsockopt(connection->sock, SOL_SOCKET, SO_ERROR, &err, &sl)) || err) {
			destroyTCPConnection(connection);
			return 0;
		};
		assert(sl == sizeof(int));
		if (-1 == event_del(connection->write_event)) {
			pthread_mutex_unlock(&connection->mutex);
			return 1;
		};
		connection->state = &tcpstate_synack_send;
		sendSynReply(connection);
	};
	pthread_mutex_unlock(&connection->mutex);
	return 0;
};

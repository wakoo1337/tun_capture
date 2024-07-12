#include <pthread.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "destroyTCPConnection.h"
#include "tcpstate_synack_send.h"

#include "tcpConnwaitEventCallback.h"
void tcpConnwaitEventCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	if (what & EV_WRITE) {
		pthread_mutex_lock(&connection->mutex);
		int err;
		socklen_t sl = sizeof(int);
		if ((-1 == getsockopt(connection->sock, SOL_SOCKET, SO_ERROR, &err, &sl)) || err) {
			pthread_mutex_unlock(&connection->mutex);
			destroyTCPConnection(connection);
			return;
		};
		assert(sl == sizeof(int));
		event_free(connection->event);
		connection->event = NULL;
		connection->state = &tcpstate_synack_send;
		sendSynReply(connection);
		pthread_mutex_unlock(&connection->mutex);
	};
};
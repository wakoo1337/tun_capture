#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"
#include "TCPConnection.h"
#include "TCPState.h"

#include "tcpCallback.h"
void tcpCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *conn;
	conn = (struct TCPConnection *) arg;
	pthread_mutex_lock(&conn->mutex);
	conn->state->event_callback(fd, what, arg);
};

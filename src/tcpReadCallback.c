#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"
#include "TCPConnection.h"
#include "TCPState.h"
#include "emergencyStop.h"

#include "tcpReadCallback.h"
void tcpReadCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *conn;
	conn = (struct TCPConnection *) arg;
	pthread_mutex_lock(&conn->mutex);
	if (conn->state->read_callback(fd, what, arg)) {
		emergencyStop(conn->context);
	};
};

#include <pthread.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "tcpSynackSendEventCallback.h"
void tcpSynackSendEventCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
};

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "tcpGotFINReadCallback.h"
unsigned int tcpGotFINReadCallback(evutil_socket_t fd, short what, void *arg) {
	struct SrcDstSockaddrs *connection = (struct SrcDstSockaddrs *) arg;
	return 0;
};

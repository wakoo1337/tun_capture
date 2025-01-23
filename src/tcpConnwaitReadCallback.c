#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <semaphore.h>
#include <event2/event.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "tcpConnwaitReadCallback.h"
unsigned int tcpConnwaitReadCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection;
	connection = (struct TCPConnection *) arg;
	pthread_mutex_unlock(&connection->mutex);
	return 0;
};

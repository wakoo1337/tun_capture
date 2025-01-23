#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "tcpLastACKWaitReadCallback.h"
unsigned int tcpLastACKWaitReadCallback(evutil_socket_t, short, void *arg) {
	struct TCPConnection *connection;
	connection = (struct TCPConnection *) arg;
	pthread_mutex_unlock(&connection->mutex);
	return 0;
};

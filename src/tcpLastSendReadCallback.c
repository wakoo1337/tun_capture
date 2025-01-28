#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "tcpLastSendReadCallback.h"
unsigned int tcpLastSendReadCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	pthread_mutex_unlock(&connection->mutex);
	return 0;
};

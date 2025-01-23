#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "MAX_APP_QUEUE.h"

#include "tcpUpdateReadEvent.h"
unsigned int tcpUpdateReadEvent(struct TCPConnection *connection) {
	if (!connection->read_finalized) {
		if (connection->app_scheduled < MAX_APP_QUEUE) {
			if (-1 == event_add(connection->read_event, NULL)) {
				return 1;
			};
		} else {
			if (-1 == event_del(connection->read_event)) {
				return 1;
			};
		};
	};
	return 0;
};

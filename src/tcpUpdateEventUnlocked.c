#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "MAX_APP_QUEUE.h"

#include "tcpUpdateEventUnlocked.h"
unsigned int tcpUpdateEventUnlocked(struct TCPConnection *connection) {
	if (connection->site_scheduled) {
		if (-1 == event_add(connection->write_event, NULL)) {
			return 1;
		};
	} else {
		if (-1 == event_del(connection->write_event)) {
			return 1;
		};
	};
	if (connection->app_scheduled < MAX_APP_QUEUE) {
		if (-1 == event_add(connection->read_event, NULL)) {
			return 1;
		};
	} else {
		if (-1 == event_del(connection->read_event)) {
			return 1;
		};
	};
	return 0;
};

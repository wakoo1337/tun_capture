#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "tcpCallback.h"
#include "MAX_APP_QUEUE.h"

#include "tcpUpdateEvent.h"
unsigned int tcpUpdateEvent(struct TCPConnection *connection) {
	event_free(connection->event);
	connection->event = event_new(connection->context->event_base, connection->sock, EV_PERSIST | ((connection->site_scheduled) ? EV_WRITE : 0) | ((connection->app_scheduled < MAX_APP_QUEUE) ? EV_READ : 0), &tcpCallback, connection);
	if (NULL == connection->event) return 1;
	if (event_add(connection->event, NULL)) {
		event_free(connection->event);
		return 1;
	};
	return 0;
};

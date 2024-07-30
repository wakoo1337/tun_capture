#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "MAX_APP_QUEUE.h"

#include "tcpUpdateEvent.h"
unsigned int tcpUpdateEvent(struct TCPConnection *connection) {
	pthread_mutex_unlock(&connection->mutex);
	pthread_mutex_lock(&connection->context->event_mutex);
	pthread_mutex_lock(&connection->mutex);
	if (connection->site_scheduled) {
		if (-1 == event_add(connection->write_event, NULL)) {
			pthread_mutex_unlock(&connection->context->event_mutex);
			return 1;
		};
	} else {
		if (-1 == event_del(connection->write_event)) {
			pthread_mutex_unlock(&connection->context->event_mutex);
			return 1;
		};
	};
	if (connection->app_scheduled < MAX_APP_QUEUE) {
		if (-1 == event_add(connection->read_event, NULL)) {
			pthread_mutex_unlock(&connection->context->event_mutex);
			return 1;
		};
	} else {
		if (-1 == event_del(connection->read_event)) {
			pthread_mutex_unlock(&connection->context->event_mutex);
			return 1;
		};
	};
	pthread_mutex_unlock(&connection->context->event_mutex);
	return 0;
};

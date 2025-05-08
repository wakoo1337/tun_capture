#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "getSendWindowSize.h"
#include "sendSiteQueueItems.h"
#include "tcpUpdateWriteEvent.h"
#include "sendTCPAcknowledgement.h"
#include "getMonotonicTimeval.h"
#include "addTimeval.h"
#include "enqueueTimeout.h"
#include "startTimer.h"
#include "tcpTimeWaitExpiredCallback.h"
#include "tcpstate_timewait.h"
#include "timewait_delay.h"

#include "tcpLastSendWriteCallback.h"
unsigned int tcpLastSendWriteCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	if (what & EV_WRITE) {
		const uint16_t old_window = getSendWindowSize(connection);
		sendSiteQueueItems(connection);
		tcpUpdateWriteEvent(connection);
		if (old_window != getSendWindowSize(connection)) sendTCPAcknowledgement(connection);
		if (NULL == connection->site_queue) {
			connection->state = &tcpstate_timewait;
			pthread_mutex_unlock(&connection->mutex);
			pthread_mutex_lock(&connection->context->timeout_mutex);
			pthread_mutex_lock(&connection->mutex);
			struct timeval now, expire;
			getMonotonicTimeval(&now);
			addTimeval(&now, &timewait_delay, &expire);
			connection->timewait_item = enqueueTimeout(connection->context, &expire, &tcpTimeWaitExpiredCallback, connection);
			startTimer(connection->context);
			pthread_mutex_unlock(&connection->context->timeout_mutex);
		};
	};
	pthread_mutex_unlock(&connection->mutex);
	return 0;
};

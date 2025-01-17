#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "getMonotonicTimeval.h"
#include "addTimeval.h"
#include "enqueueTimeout.h"
#include "startTimer.h"
#include "tcpTimeWaitExpiredCallback.h"
#include "tcpstate_timewait.h"
#include "timewait_delay.h"

#include "tcpRecvZeroOnFIN.h"
unsigned int tcpRecvZeroOnFIN(struct TCPConnection *connection) {
	connection->first_desired++;
	connection->state = &tcpstate_timewait;
	pthread_mutex_unlock(&connection->mutex);
	pthread_mutex_lock(&connection->context->timeout_mutex);
	pthread_mutex_lock(&connection->mutex);
	struct timeval now, expire;
	getMonotonicTimeval(&now);
	addTimeval(&timewait_delay, &now, &expire);
	struct TimeoutItem *timeout_item;
	timeout_item = enqueueTimeout(connection->context, &expire, &tcpTimeWaitExpiredCallback, connection, &connection->mutex);
	startTimer(connection->context);
	pthread_mutex_unlock(&connection->context->timeout_mutex);
	return 0;
}; 

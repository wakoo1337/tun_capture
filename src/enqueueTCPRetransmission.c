#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"
#include "TimeoutItem.h"
#include "getMonotonicTimeval.h"
#include "addTimeval.h"
#include "enqueueTimeout.h"
#include "tcpRetransmissionTimerCallback.h"
#include "startTimer.h"
#include "retry_delay.h"

#include "enqueueTCPRetransmission.h"
unsigned int enqueueTCPRetransmission(struct TCPAppQueueItem *item) {
	pthread_mutex_unlock(&item->connection->mutex);
	pthread_mutex_lock(&item->connection->context->timeout_mutex);
	pthread_mutex_lock(&item->connection->mutex);
	if (item->timeout) { // cancelTimeout тут не подойдёт: удаление старого таймаута и установка нового должны происходить атомарно
		item->timeout->is_del = true;
		item->timeout = NULL;
	};
	struct timeval now, expire;
	getMonotonicTimeval(&now);
	addTimeval(&now, &retry_delay, &expire);
	item->timeout = enqueueTimeout(item->connection->context, &expire, &tcpRetransmissionTimerCallback, item, &item->connection->mutex);
	const unsigned int result = (NULL == item->timeout);
	if (0 == result) startTimer(item->connection->context);
	pthread_mutex_unlock(&item->connection->context->timeout_mutex);
	return result;
};

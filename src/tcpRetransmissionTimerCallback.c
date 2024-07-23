#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "contrib/heap.h"
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"
#include "getMonotonicTimeval.h"
#include "compareTimeval.h"
#include "sendTCPPacket.h"
#include "addTimeval.h"
#include "enqueueTimeout.h"
#include "checkByteInWindow.h"
#include "retry_delay.h"

#include "tcpRetransmissionTimerCallback.h"
void tcpRetransmissionTimerCallback(void *arg) {
	struct TCPAppQueueItem *item;
	item = (struct TCPAppQueueItem *) arg;
	pthread_mutex_lock(&item->connection->mutex);
	struct timeval now, timeout;
	if (checkByteInWindow(item->connection->latest_ack, item->connection->app_window, item->confirm_ack - item->data_size) && checkByteInWindow(item->connection->latest_ack, item->connection->app_window, item->confirm_ack)) sendTCPPacket(item->connection, item, false);
	getMonotonicTimeval(&now);
	addTimeval(&now, &retry_delay, &timeout);
	item->timeout = enqueueTimeout(item->connection->context, &timeout, &tcpRetransmissionTimerCallback, item);
	pthread_mutex_unlock(&item->connection->mutex);
};

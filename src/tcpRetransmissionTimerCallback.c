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
#include "enqueueTCPPacketTransmission.h"
#include "addTimeval.h"
#include "enqueueTimeout.h"
#include "checkByteInWindow.h"
#include "retry_delay.h"

#include "tcpRetransmissionTimerCallback.h"
void tcpRetransmissionTimerCallback(void *arg) {
	struct TCPAppQueueItem *item;
	item = (struct TCPAppQueueItem *) arg;
	if (item->is_filled) {
		struct timeval now, timeout;
		if (checkByteInWindow(item->connection->latest_ack, item->connection->app_window, item->confirm_ack - item->data_size) && checkByteInWindow(item->connection->latest_ack, item->connection->app_window, item->confirm_ack)){
			item->ref_count++;
			enqueueTCPPacketTransmission(item->connection, item);
		};
		if (item->timeout) { // Если таймер не был отменён
			pthread_mutex_unlock(&item->connection->mutex);
			pthread_mutex_lock(&item->connection->context->timeout_mutex);
			getMonotonicTimeval(&now);
			addTimeval(&now, &retry_delay, &timeout);
			item->timeout = enqueueTimeout(item->connection->context, &timeout, &tcpRetransmissionTimerCallback, item, &item->connection->mutex);
			pthread_mutex_lock(&item->connection->mutex);
			pthread_mutex_unlock(&item->connection->context->timeout_mutex);
		};
	};
};

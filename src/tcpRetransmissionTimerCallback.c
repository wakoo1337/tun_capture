#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "TCPAppQueueItem.h"
#include "TCPConnection.h"
#include "getMonotonicTimeval.h"
#include "compareTimeval.h"
#include "sendTCPPacket.h"
#include "addTimeval.h"
#include "retry_delay.h"

#include "tcpRetransmissionTimerCallback.h"
void tcpRetransmissionTimerCallback(void *arg) {
	struct TCPConnection *connection;
	connection = (struct TCPConnection *) arg;
	pthread_mutex_lock(&connection->mutex);
	struct TCPAppQueueItem *current;
	current = connection->app_queue;
	while (current) {
		struct timeval now;
		getMonotonicTimeval(&now);
		if (compareTimeval(&now, &current->retry) >= 0) {
			sendTCPPacket(connection->context, current);
			addTimeval(&now, &retry_delay, &current->retry);
		};
		current = current->next;
	};
	pthread_mutex_unlock(&connection->mutex);
};

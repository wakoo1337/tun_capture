#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"
#include "enqueueTCPPacketTransmission.h"
#include "enqueueTCPRetransmission.h"
#include "isAppQueueItemInWindow.h"

#include "tcpRetransmissionTimerCallback.h"
void tcpRetransmissionTimerCallback(void *arg) {
	struct TCPAppQueueItem *item;
	item = (struct TCPAppQueueItem *) arg;
	if (item->is_filled && isAppQueueItemInWindow(item->connection->latest_ack, item->connection->app_window, item)) {
		item->ref_count++;
		enqueueTCPPacketTransmission(item);
		if (item->timeout) enqueueTCPRetransmission(item);
	};
};

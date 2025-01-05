#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPAppQueueItem.h"
#include "TCPConnection.h"
#include "isAppQueueItemInWindow.h"
#include "enqueueTCPPacketTransmission.h"
#include "enqueueTCPRetransmission.h"

#include "enqueueUnsentTCPPacketsTransmission.h"
unsigned int enqueueUnsentTCPPacketsTransmission(struct TCPConnection *connection) {
	struct TCPAppQueueItem *appqueue_current;
	appqueue_current = connection->app_queue;
	while (appqueue_current && appqueue_current->is_filled) {
		if ((appqueue_current->timeout == NULL) && isAppQueueItemInWindow(connection->latest_ack, connection->app_window, appqueue_current)) {
			unsigned int status = 0;
			status |= enqueueTCPPacketTransmission(appqueue_current);
			if (status) return status;
			status |= enqueueTCPRetransmission(appqueue_current);
			if (status) return status;
		};
		appqueue_current = appqueue_current->next;
	};
	return 0;
};

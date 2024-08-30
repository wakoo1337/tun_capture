#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPSiteQueueItem.h"
#include "TCPConnection.h"
#include "getSendWindowSize.h"
#include "sendTCPAcknowledgement.h"
#include "sendSiteQueueItems.h"
#include "tcpUpdateWriteEvent.h"

#include "tcpEstablishedWriteCallback.h"
unsigned int tcpEstablishedWriteCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	if (what & EV_WRITE) {
		const uint16_t old_window = getSendWindowSize(connection);
		sendSiteQueueItems(connection);
		tcpUpdateWriteEvent(connection);
		if (old_window != getSendWindowSize(connection)) sendTCPAcknowledgement(connection);
	};
	pthread_mutex_unlock(&connection->mutex);
	return 0;
};

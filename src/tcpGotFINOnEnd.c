#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "enqueueStubTCPPacketQueueItem.h"
#include "tcpstate_lastackwait.h"

#include "tcpGotFINOnEnd.h"
unsigned int tcpGotFINOnEnd(struct TCPConnection *connection) {
	if ((NULL == connection->site_queue) && (NULL == connection->app_queue)) {
		connection->should_send_fin = true;
		connection->fin_seq = connection->our_seq + connection->app_scheduled;
		enqueueStubTCPPacketQueueItem(connection);
		connection->state = &tcpstate_lastackwait;
	} else {
		event_del(connection->read_event);
	};
	return 0;
};

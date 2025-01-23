#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "tcpFinalizeRead.h"
#include "enqueueStubTCPPacketQueueItem.h"
#include "tcpstate_recvzero.h"

#include "tcpEstablishedOnEnd.h"
unsigned int tcpEstablishedOnEnd(struct TCPConnection *connection) {
	connection->state = &tcpstate_recvzero;
	connection->should_send_fin = true;
	connection->fin_seq = connection->our_seq + connection->app_scheduled;
	enqueueStubTCPPacketQueueItem(connection);
	return tcpFinalizeRead(connection);
};

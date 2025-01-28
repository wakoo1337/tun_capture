#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "tcpFinalizeRead.h"
#include "sendTCPFinalize.h"
#include "tcpstate_recvzero.h"

#include "tcpEstablishedOnEnd.h"
unsigned int tcpEstablishedOnEnd(struct TCPConnection *connection) {
	sendTCPFinalize(connection);
	connection->first_desired++;
	connection->state = &tcpstate_recvzero;
	return tcpFinalizeRead(connection);
};

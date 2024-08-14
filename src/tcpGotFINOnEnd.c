#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "sendTCPFinalize.h"

#include "tcpGotFINOnEnd.h"
unsigned int tcpGotFINOnEnd(struct TCPConnection *connection) {
	shutdown(connection->sock, SHUT_RD);
	unsigned int value;
	connection->first_desired++;
	value = sendTCPFinalize(connection);
	// TODO сменить состояние
	return value;
};

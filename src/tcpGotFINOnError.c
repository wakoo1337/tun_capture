#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "tcpFinalizeRead.h"
#include "tcpFinalizeWrite.h"
#include "tcpstate_connreset.h"

#include "tcpGotFINOnError.h"
unsigned int tcpGotFINOnError(struct TCPConnection *connection) {
	if (errno == ECONNRESET) {
		connection->state = &tcpstate_connreset;
		tcpFinalizeRead(connection);
		tcpFinalizeWrite(connection);
	};
	return 0;
};

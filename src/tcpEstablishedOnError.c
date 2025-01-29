#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "tcpFinalizeRead.h"
#include "tcpFinalizeWrite.h"
#include "tcpstate_connreset.h"

#include "tcpEstablishedOnError.h"
unsigned int tcpEstablishedOnError(struct TCPConnection *connection) {
	switch (errno) {
		case ECONNABORTED:
		case EHOSTUNREACH:
		case ECONNREFUSED:
		case ECONNRESET:
		case EPIPE:
		case ETIMEDOUT:
			connection->state = &tcpstate_connreset;
			tcpFinalizeRead(connection);
			tcpFinalizeWrite(connection);
		case ESHUTDOWN:
			return 0;
		break;
		default:
			return 1;
	};
	return 0;
};

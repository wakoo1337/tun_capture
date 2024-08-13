#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "tcpEstablishedOnError.h"
void tcpEstablishedOnError(struct TCPConnection *connection) {
	return;
};

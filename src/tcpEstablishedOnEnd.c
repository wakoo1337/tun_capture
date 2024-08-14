#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "tcpEstablishedOnEnd.h"
unsigned int tcpEstablishedOnEnd(struct TCPConnection *connection) {
	return 0;
};

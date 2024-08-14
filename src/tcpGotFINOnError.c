#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "tcpGotFINOnError.h"
unsigned int tcpGotFINOnError(struct TCPConnection *connection) {
	return 0;
};

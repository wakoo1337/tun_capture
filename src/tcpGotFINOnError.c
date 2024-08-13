#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "tcpGotFINOnError.h"
void tcpGotFINOnError(struct TCPConnection *connection) {
	return;
};

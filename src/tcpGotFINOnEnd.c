#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "tcpGotFINOnEnd.h"
void tcpGotFINOnEnd(struct TCPConnection *connection) {
	return;
};

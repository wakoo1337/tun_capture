#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "tcpConnwaitPacketsProcessor.h"
unsigned int tcpConnwaitPacketsProcessor(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	free(payload->free_me);
	return 0;
};

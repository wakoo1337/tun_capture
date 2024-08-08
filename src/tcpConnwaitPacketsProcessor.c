#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include "RefcountBuffer.h"
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "decrementRefcount.h"

#include "tcpConnwaitPacketsProcessor.h"
unsigned int tcpConnwaitPacketsProcessor(struct TCPConnection *connection, struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	decrementRefcount(&payload->buffer);
	return 0;
};

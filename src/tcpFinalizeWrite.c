#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "tcpWriteFinalizer.h"

#include "tcpFinalizeWrite.h"
unsigned int tcpFinalizeWrite(struct TCPConnection *connection) {
	if (!connection->write_finalized) return event_free_finalize(0, connection->write_event, &tcpWriteFinalizer);
	else return 0;
};

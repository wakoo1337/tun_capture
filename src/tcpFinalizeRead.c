#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "tcpReadFinalizer.h"

#include "tcpFinalizeRead.h"
unsigned int tcpFinalizeRead(struct TCPConnection *connection) {
	if (!connection->read_finalized) return event_free_finalize(0, connection->read_event, &tcpReadFinalizer);
	else return 0;
};

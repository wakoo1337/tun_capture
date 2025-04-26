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
	if (connection->read_event && (!connection->read_finalize_called)) {
		const unsigned int result = event_free_finalize(0, connection->read_event, &tcpReadFinalizer);
		connection->read_finalize_called = true;
		return result;
	} else return 0;
};

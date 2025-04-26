#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "tcpWriteFinalizer.h"

#include "tcpFinalizeWrite.h"
unsigned int tcpFinalizeWrite(struct TCPConnection *connection) {
	if (connection->write_event && (!connection->write_finalize_called)) {
		const unsigned int result = event_free_finalize(0, connection->write_event, &tcpWriteFinalizer);
		connection->write_finalize_called = true;
		return result;
	} else return 0;
};

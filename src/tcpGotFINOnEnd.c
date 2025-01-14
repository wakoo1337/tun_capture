#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "sendTCPFinalize.h"
#include "tcpstate_lastackwait.h"

#include "tcpGotFINOnEnd.h"
unsigned int tcpGotFINOnEnd(struct TCPConnection *connection) {
	event_del(connection->read_event);
	if (NULL == connection->app_queue) {
		unsigned int value;
		value = sendTCPFinalize(connection);
		connection->first_desired++;
		connection->state = &tcpstate_lastackwait;
		return value;
	} else return 0;
};

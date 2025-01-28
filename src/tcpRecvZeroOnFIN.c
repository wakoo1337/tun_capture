#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "tcpstate_lastsend.h"

#include "tcpRecvZeroOnFIN.h"
unsigned int tcpRecvZeroOnFIN(struct TCPConnection *connection) {
	connection->first_desired++;
	connection->state = &tcpstate_lastsend;
	event_add(connection->write_event, NULL);
	return 0;
}; 

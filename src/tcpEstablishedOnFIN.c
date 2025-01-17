#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPHeaderData.h"
#include "IPPacketPayload.h"
#include "TCPConnection.h"
#include "TCPState.h"
#include "tcpstate_gotfin.h"

#include "tcpEstablishedOnFIN.h"
unsigned int tcpEstablishedOnFIN(struct TCPConnection *connection) {
	connection->first_desired++;
	connection->state = &tcpstate_gotfin;
	return (unsigned int) event_add(connection->write_event, NULL);
};

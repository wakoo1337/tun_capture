#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"
#include "tcpCleanupConfirmed.h"
#include "tcpFinalizeRead.h"
#include "tcpFinalizeWrite.h"
#include "isNewAckAcceptable.h"

#include "tcpLastACKWaitPacketsProcessor.h"
unsigned int tcpLastACKWaitPacketsProcessor(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	free(payload->free_me);
	if (isNewAckAcceptable(connection, header->ack_num)) {
		connection->latest_ack = header->ack_num;
		tcpCleanupConfirmed(connection);
	};
	if (NULL == connection->app_queue) {
		return tcpFinalizeRead(connection) || tcpFinalizeWrite(connection);
	};
	return 0;
};

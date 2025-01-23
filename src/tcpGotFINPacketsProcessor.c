#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "TCPHeaderData.h"
#include "IPPacketPayload.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "isNewAckAcceptable.h"
#include "tcpCleanupConfirmed.h"
#include "enqueueUnsentTCPPacketsTransmission.h"

#include "tcpGotFINPacketsProcessor.h"
unsigned int tcpGotFINPacketsProcessor(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	free(payload->free_me);
	if (isNewAckAcceptable(connection, header->ack_num)) {
		connection->latest_ack = header->ack_num;
		tcpCleanupConfirmed(connection);
		enqueueUnsentTCPPacketsTransmission(connection);
		if (NULL == connection->app_queue) event_add(connection->read_event, NULL);
	};
	return 0;
};

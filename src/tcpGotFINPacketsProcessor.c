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
#include "scaleRemoteWindow.h"
#include "tcpUpdateReadEvent.h"

#include "tcpGotFINPacketsProcessor.h"
unsigned int tcpGotFINPacketsProcessor(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	free(payload->free_me);
	if (isNewAckAcceptable(connection, header->ack_num)) {
		connection->latest_ack = header->ack_num;
		connection->app_window = scaleRemoteWindow(connection, header->raw_window);
		tcpCleanupConfirmed(connection);
		enqueueUnsentTCPPacketsTransmission(connection);
		tcpUpdateReadEvent(connection);
	};
	return 0;
};

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
#include "isNewAckAcceptable.h"
#include "enqueueUnsentTCPPacketsTransmission.h"
#include "scaleRemoteWindow.h"

#include "tcpLastACKWaitPacketsProcessor.h"
unsigned int tcpLastACKWaitPacketsProcessor(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	free(payload->free_me);
	if (isNewAckAcceptable(connection, header->ack_num)) {
		connection->latest_ack = header->ack_num;
		connection->app_window = scaleRemoteWindow(connection, header->raw_window);
		tcpCleanupConfirmed(connection);
		enqueueUnsentTCPPacketsTransmission(connection);
	};
	if (NULL == connection->app_queue) {
		return tcpFinalizeRead(connection);
	};
	return 0;
};

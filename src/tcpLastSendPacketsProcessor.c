#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"
#include "isNewAckAcceptable.h"
#include "scaleRemoteWindow.h"
#include "tcpCleanupConfirmed.h"
#include "enqueueUnsentTCPPacketsTransmission.h"
#include "sendTCPAcknowledgement.h"

#include "tcpLastSendPacketsProcessor.h"
unsigned int tcpLastSendPacketsProcessor(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	free(payload->free_me);
	if (isNewAckAcceptable(connection, header->ack_num)) { // Обновляем последний ACK и размер окна
		connection->latest_ack = header->ack_num;
		connection->app_window = scaleRemoteWindow(connection, header->raw_window);
		tcpCleanupConfirmed(connection);
		enqueueUnsentTCPPacketsTransmission(connection);
	} else sendTCPAcknowledgement(connection);
	return 0;
};

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"
#include "checkPacketInRecieveWindow.h"
#include "isNewAckAcceptable.h"
#include "addPacketToPrequeue.h"
#include "tcpCleanupConfirmed.h"
#include "prequeueToSiteQueue.h"
#include "enqueueUnsentTCPPacketsTransmission.h"
#include "tcpUpdateWriteEvent.h"
#include "sendTCPAcknowledgement.h"
#include "tcpRecvZeroOnFIN.h"
#include "scaleRemoteWindow.h"
#include "tcpstate_recvzero.h"

#include "tcpRecvZeroPacketsProcessor.h"
unsigned int tcpRecvZeroPacketsProcessor(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	const uint32_t old_first = connection->first_desired;
	if (!checkPacketInRecieveWindow(connection, payload, header)) {
		// Если не все байты сегмента лежат в окне приёма, он игнорируется
		sendTCPAcknowledgement(connection);
		free(payload->free_me);
		return 0;
	};
	if (addPacketToPrequeue(connection, payload, header)) return 1;
	if (isNewAckAcceptable(connection, header->ack_num)) { // Обновляем последний ACK и размер окна
		connection->latest_ack = header->ack_num;
		connection->app_window = scaleRemoteWindow(connection, header->raw_window);
	};
	tcpCleanupConfirmed(connection);
	if (prequeueToSiteQueue(connection, &tcpRecvZeroOnFIN)) return 1;
	enqueueUnsentTCPPacketsTransmission(connection);
	if (connection->state == &tcpstate_recvzero) tcpUpdateWriteEvent(connection);
	if (old_first != connection->first_desired) sendTCPAcknowledgement(connection);
	return 0;
};

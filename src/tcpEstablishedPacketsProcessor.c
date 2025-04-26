#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "contrib/avl.h"
#include "IPPacketPayload.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPHeaderData.h"
#include "TCPSitePrequeueItem.h"
#include "prequeueItemToSiteData.h"
#include "tcpUpdateReadEvent.h"
#include "tcpUpdateWriteEvent.h"
#include "sendTCPAcknowledgement.h"
#include "tcpCleanupConfirmed.h"
#include "isNewAckAcceptable.h"
#include "scaleRemoteWindow.h"
#include "enqueueUnsentTCPPacketsTransmission.h"
#include "checkPacketInRecieveWindow.h"
#include "tcpEstablishedOnFIN.h"
#include "addPacketToPrequeue.h"
#include "prequeueToSiteQueue.h"
#include "processTCPPacketImmediately.h"
#include "tcpstate_established.h"

#include "tcpEstablishedPacketsProcessor.h"
unsigned int tcpEstablishedPacketsProcessor(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	const uint32_t old_first = connection->first_desired;
	if (!checkPacketInRecieveWindow(connection, payload, header)) {
		// Если не все байты сегмента лежат в окне приёма, он игнорируется
		free(payload->free_me);
		return sendTCPAcknowledgement(connection);
	};
	if (payload->count <= header->data_offset) free(payload->free_me);
	if (header->seq_num == connection->first_desired) {
		if (processTCPPacketImmediately(connection, payload, header, &tcpEstablishedOnFIN)) return 1;
	} else {
		if (addPacketToPrequeue(connection, payload, header)) return 1;
		if (prequeueToSiteQueue(connection, &tcpEstablishedOnFIN)) return 1;
	};
	if (isNewAckAcceptable(connection, header->ack_num)) { // Обновляем последний ACK и размер окна
		connection->latest_ack = header->ack_num;
		connection->app_window = scaleRemoteWindow(connection, header->raw_window);
	};
	tcpCleanupConfirmed(connection);
	enqueueUnsentTCPPacketsTransmission(connection);
	tcpUpdateReadEvent(connection);
	if (connection->state == &tcpstate_established) tcpUpdateWriteEvent(connection);
	if (old_first != connection->first_desired) sendTCPAcknowledgement(connection);
	return 0;
};

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "IPPacketPayload.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPHeaderData.h"
#include "TimeoutItem.h"
#include "tcpCleanupConfirmed.h"
#include "tcpUpdateReadEvent.h"
#include "tcpUpdateWriteEvent.h"
#include "scaleRemoteWindow.h"
#include "tcpstate_established.h"

#include "tcpSynackSendPacketsProcessor.h"
unsigned int tcpSynackSendPacketsProcessor(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	if ((header->seq_num == connection->first_desired) && (header->ack_num == connection->our_seq+1) && (header->ack)) {
		connection->latest_ack = header->ack_num;
		connection->app_window = scaleRemoteWindow(connection, header->raw_window);
		tcpCleanupConfirmed(connection);
		connection->our_seq++;
		connection->state = &tcpstate_established;
		tcpUpdateReadEvent(connection);
		tcpUpdateWriteEvent(connection);
	};
	free(payload->free_me); // TODO убрать, тут тоже могут быть данные
	return 0;
};

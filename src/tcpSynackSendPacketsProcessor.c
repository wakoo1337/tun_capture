#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include "IPPacketPayload.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPHeaderData.h"
#include "cancelTimeout.h"
#include "tcpCleanupConfirmed.h"
#include "tcpstate_established.h"

#include "tcpSynackSendPacketsProcessor.h"
unsigned int tcpSynackSendPacketsProcessor(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	if ((header->seq_num == connection->first_desired) && (header->ack_num == connection->our_seq+1) && (header->ack)) {
		connection->state = &tcpstate_established;
		connection->our_seq++;
		cancelTimeout(connection->context, &connection->timer);
		tcpCleanupConfirmed(connection, header->ack_num, header->raw_window);
	};
	free(payload->free_me); // TODO убрать, тут тоже могут быть данные
	return 0;
};

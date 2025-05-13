#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "tcpDataProcessor.h"
#include "sendTCPFinalize.h"

#include "processTCPData.h"
unsigned int processTCPData(struct CaptureContext *context, uint8_t *packet, unsigned int count, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	assert(connection->context == context);
	unsigned int result;
	result = tcpDataProcessor(connection, packet, count, false);
	if (result) return 1;
	if (connection->should_send_fin && (connection->fin_seq == connection->seq_first)) {
		sendTCPFinalize(connection);
		connection->seq_next++;
		connection->seq_first++;
	};
	return 0;
};

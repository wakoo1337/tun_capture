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

#include "processTCPUrgentData.h"
unsigned int processTCPUrgentData(struct CaptureContext *context, uint8_t *packet, unsigned int count, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	assert(connection->context == context);
	return tcpDataProcessor(connection, packet, count, true);
};

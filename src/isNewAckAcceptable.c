#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "checkByteInWindow.h"

#include "isNewAckAcceptable.h"
bool isNewAckAcceptable(struct TCPConnection *connection, uint32_t new_ack) {
	return checkByteInWindow(connection->latest_ack, connection->app_scheduled + 1, new_ack) || (connection->should_send_fin && (new_ack == (connection->fin_seq + 1)));
};

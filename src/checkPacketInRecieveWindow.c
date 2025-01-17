#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPHeaderData.h"
#include "IPPacketPayload.h"
#include "checkByteInWindow.h"
#include "MAX_SITE_QUEUE.h"

#include "checkPacketInRecieveWindow.h"
bool checkPacketInRecieveWindow(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	const uint32_t last = header->seq_num + (payload->count - header->data_offset);
	return (checkByteInWindow(connection->first_desired, MAX_SITE_QUEUE - connection->site_scheduled, header->seq_num) && checkByteInWindow(connection->first_desired, MAX_SITE_QUEUE - connection->site_scheduled, last));
};

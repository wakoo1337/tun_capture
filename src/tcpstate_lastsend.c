#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"
#include "TCPState.h"
#include "tcpLastSendReadCallback.h"
#include "tcpLastSendWriteCallback.h"
#include "tcpLastSendPacketsProcessor.h"

#include "tcpstate_lastsend.h"
const struct TCPState tcpstate_lastsend = {
	.read_callback = &tcpLastSendReadCallback,
	.write_callback = &tcpLastSendWriteCallback,
	.packets_processor = &tcpLastSendPacketsProcessor,
};

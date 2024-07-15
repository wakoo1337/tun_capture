#include <sys/socket.h>
#include <stdint.h>
#include <stdbool.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "IPPacketPayload.h"
#include "TCPConnection.h"
#include "TCPHeaderData.h"
#include "TCPState.h"
#include "tcpSynackSendEventCallback.h"
#include "tcpSynackSendPacketsProcessor.h"

#include "tcpstate_synack_send.h"
const struct TCPState tcpstate_synack_send = {
	.event_callback = &tcpSynackSendEventCallback,
	.packets_processor = &tcpSynackSendPacketsProcessor
};

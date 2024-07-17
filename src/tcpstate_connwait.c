#include <stdint.h>
#include <stdbool.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"
#include "TCPConnection.h"
#include "TCPState.h"
#include "tcpConnwaitEventCallback.h"
#include "tcpConnwaitPacketsProcessor.h"

#include "tcpstate_connwait.h"
const struct TCPState tcpstate_connwait = {
	.event_callback = &tcpConnwaitEventCallback,
	.packets_processor = &tcpConnwaitPacketsProcessor
};

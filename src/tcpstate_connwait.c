#include <stdint.h>
#include <stdbool.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"
#include "TCPConnection.h"
#include "TCPState.h"
#include "tcpConnwaitReadCallback.h"
#include "tcpConnwaitWriteCallback.h"
#include "tcpConnwaitPacketsProcessor.h"

#include "tcpstate_connwait.h"
const struct TCPState tcpstate_connwait = {
	.read_callback = &tcpConnwaitReadCallback,
	.write_callback = &tcpConnwaitWriteCallback,
	.packets_processor = &tcpConnwaitPacketsProcessor
};

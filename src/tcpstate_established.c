#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "IPPacketPayload.h"
#include "SrcDstSockaddrs.h"
#include "TCPHeaderData.h"
#include "TCPConnection.h"
#include "TCPState.h"
#include "tcpEstablishedReadCallback.h"
#include "tcpEstablishedWriteCallback.h"
#include "tcpEstablishedPacketsProcessor.h"

#include "tcpstate_established.h"
const struct TCPState tcpstate_established = {
	.read_callback = &tcpEstablishedReadCallback,
	.write_callback = &tcpEstablishedWriteCallback,
	.packets_processor = &tcpEstablishedPacketsProcessor
};

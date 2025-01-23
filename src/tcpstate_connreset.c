#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "IPPacketPayload.h"
#include "SrcDstSockaddrs.h"
#include "TCPHeaderData.h"
#include "TCPConnection.h"
#include "TCPState.h"
#include "tcpConnResetReadCallback.h"
#include "tcpConnResetWriteCallback.h"
#include "tcpConnResetPacketsProcessor.h"

#include "tcpstate_connreset.h"
const struct TCPState tcpstate_connreset = {
	.read_callback = &tcpConnResetReadCallback,
	.write_callback = &tcpConnResetWriteCallback,
	.packets_processor = &tcpConnResetPacketsProcessor
};

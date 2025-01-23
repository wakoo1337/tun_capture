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
#include "tcpLastACKWaitReadCallback.h"
#include "tcpLastACKWaitWriteCallback.h"
#include "tcpLastACKWaitPacketsProcessor.h"

#include "tcpstate_lastackwait.h"
const struct TCPState tcpstate_lastackwait = {
	.read_callback = &tcpLastACKWaitReadCallback,
	.write_callback = &tcpLastACKWaitWriteCallback,
	.packets_processor = &tcpLastACKWaitPacketsProcessor
};

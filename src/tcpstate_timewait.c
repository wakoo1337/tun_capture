#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPHeaderData.h"
#include "IPPacketPayload.h"
#include "TCPState.h"
#include "tcpTimeWaitReadCallback.h"
#include "tcpTimeWaitWriteCallback.h"
#include "tcpTimeWaitPacketsProcessor.h"

#include "tcpstate_timewait.h"
const struct TCPState tcpstate_timewait = {
	.read_callback = tcpTimeWaitReadCallback,
	.write_callback = tcpTimeWaitWriteCallback,
	.packets_processor = tcpTimeWaitPacketsProcessor
};

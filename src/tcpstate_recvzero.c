#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"
#include "TCPState.h"
#include "tcpRecvZeroReadCallback.h"
#include "tcpRecvZeroWriteCallback.h"
#include "tcpRecvZeroPacketsProcessor.h"

#include "tcpstate_recvzero.h"
const struct TCPState tcpstate_recvzero = {
	.read_callback = &tcpRecvZeroReadCallback,
	.write_callback = &tcpRecvZeroWriteCallback,
	.packets_processor = &tcpRecvZeroPacketsProcessor
};

#include <sys/socket.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "IPPacketPayload.h"
#include "TCPConnection.h"
#include "TCPHeaderData.h"
#include "TCPState.h"
#include "tcpSynackSendReadCallback.h"
#include "tcpSynackSendWriteCallback.h"
#include "tcpSynackSendPacketsProcessor.h"

#include "tcpstate_synack_send.h"
const struct TCPState tcpstate_synack_send = {
	.read_callback = &tcpSynackSendReadCallback,
	.write_callback = &tcpSynackSendWriteCallback,
	.packets_processor = &tcpSynackSendPacketsProcessor
};

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
//#include "tcpGotFINReadCallback.h"
//#include "tcpGotFINWriteCallback.h"
//#include "tcpGotFINPacketsProcessor.h"

#include "tcpstate_gotfin.h"
const struct TCPState tcpstate_gotfin = {
	//.read_callback = &tcpGotFINReadCallback,
	//.write_callback = &tcpGotFINWriteCallback,
	//.packets_processor = &tcpGotFINPacketsProcessor
};

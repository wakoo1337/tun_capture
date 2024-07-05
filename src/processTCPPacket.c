#include <stdint.h>
#include <sys/socket.h>
#include <pthread.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "CaptureContext.h"
#include "IPPacketPayload.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "SrcDstSockaddrs.h"
#include "TCPHeaderData.h"
#include "parseTCPHeader.h"

#include "processTCPPacket.h"
unsigned int processTCPPacket(struct CaptureContext *context, const struct IPPacketPayload *payload, const struct NetworkProtocolStrategy *strategy, struct SrcDstSockaddrs *addrs) {
	struct TCPHeaderData hdr;
	if (parseTCPHeader(&hdr, payload->packet, payload->count, payload->pseudo, strategy->pseudo_length)) return 1;
	return 0;
};

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "IPv4HeaderData.h"
#include "IPPacketPayload.h"
#include "SrcDstSockaddrs.h"
#include "RefcountBuffer.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "parseIPv4Header.h"
#include "processUDPPacket.h"
#include "setIPv4SockaddrPort.h"
#include "ipv4_strategy.h"
#include "processTCPPacket.h"
#include "decrementRefcount.h"

#include "processIPv4Packet.h"
unsigned int processIPv4Packet(struct CaptureContext *context, struct RefcountBuffer *buffer) {
	struct IPv4HeaderData hdr;
	uint8_t pseudo[12];
	if (parseIPv4Header(&hdr, &buffer->data[buffer->netpkt_offset], buffer->size - buffer->netpkt_offset, pseudo) || (hdr.total_length != buffer->size)) {
		decrementRefcount(&buffer);
		return 0;
	};
	buffer->trapkt_offset = hdr.len;
	if ((hdr.fragment_offset == 0) && (!hdr.mf)) {
		struct SrcDstSockaddrs srcdst = {0};
		((struct sockaddr_in *) &(srcdst.src))->sin_family = AF_INET;
		((struct sockaddr_in *) &(srcdst.src))->sin_addr.s_addr = htonl(hdr.src.s_addr);
		((struct sockaddr_in *) &(srcdst.dst))->sin_family = AF_INET;
		((struct sockaddr_in *) &(srcdst.dst))->sin_addr.s_addr = htonl(hdr.dst.s_addr);
		struct IPPacketPayload ip_payload = {
			.buffer = buffer,
			.pseudo = pseudo,
		};
		if (hdr.protocol == 6) {
			// TCP
			return processTCPPacket(context, &ip_payload, &ipv4_strategy, &srcdst);
		} else if (hdr.protocol == 17) {
			// UDP
			return processUDPPacket(context, &ip_payload, &ipv4_strategy, &srcdst);
		}
		return 0;
	} else {
		// Пакет фрагментирован. TODO: реализовать дефрагментацию
		decrementRefcount(&buffer);
		return 0;
	};
};

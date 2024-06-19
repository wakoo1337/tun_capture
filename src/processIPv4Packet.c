#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "CaptureContext.h"
#include "IPv4HeaderData.h"
#include "IPPacketPayload.h"
#include "SrcDstSockaddrs.h"
#include "UDPNetworkProtocolStrategy.h"
#include "parseIPv4Header.h"
#include "processUDPPacket.h"
#include "setIPv4SockaddrPort.h"

#include "processIPv4Packet.h"
unsigned int processIPv4Packet(struct CaptureContext *context, void *data, unsigned int count) {
	struct IPv4HeaderData hdr;
	uint8_t pseudo[12];
	if (parseIPv4Header(&hdr, data, count, pseudo)) return 1;
	if (hdr.total_length != count) return 1;
	if ((hdr.fragment_offset == 0) && (!hdr.mf)) {
		if (hdr.protocol == 6) {
			// TCP
		} else if (hdr.protocol == 17) {
			// UDP
			struct SrcDstSockaddrs srcdst = {0};
			((struct sockaddr_in *) &(srcdst.src))->sin_family = AF_INET;
			((struct sockaddr_in *) &(srcdst.src))->sin_addr.s_addr = htonl(hdr.src.s_addr);
			((struct sockaddr_in *) &(srcdst.dst))->sin_family = AF_INET;
			((struct sockaddr_in *) &(srcdst.dst))->sin_addr.s_addr = htonl(hdr.dst.s_addr);
			static const struct UDPNetworkProtocolStrategy strategy = {
				.pseudo_length = 12,
				.port_setter = &setIPv4SockaddrPort
			};
			struct IPPacketPayload ip_payload = {
				.free_me = data,
				.packet = data + hdr.len,
				.pseudo = pseudo,
				.count = hdr.total_length - hdr.len
			};
			return processUDPPacket(context, &ip_payload, &strategy, &srcdst);
		}
		return 0;
	} else {
		// Пакет фрагментирован. TODO: реализовать дефрагментацию
		return 1;
	};
};

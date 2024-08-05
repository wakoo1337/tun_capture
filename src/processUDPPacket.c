#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <event2/event.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "IPPacketPayload.h"
#include "SrcDstSockaddrs.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "UDPHeaderData.h"
#include "UDPStackItem.h"
#include "ChecksumContext.h"
#include "UDPBinding.h"
#include "get16Bit.h"
#include "set16Bit.h"
#include "initChecksum.h"
#include "computeChecksum.h"
#include "getChecksum.h"
#include "findUDPBinding.h"

#include "processUDPPacket.h"
unsigned int processUDPPacket(struct CaptureContext *context, const struct IPPacketPayload *payload, const struct NetworkProtocolStrategy *strategy, struct SrcDstSockaddrs *addrs) {
	struct UDPHeaderData hdr;
	if (payload->count < 8) {
		free(payload->free_me);
		return 0;
	};
	hdr.src_port = ntohs(get16Bit(&payload->packet[0]));
	hdr.dst_port = ntohs(get16Bit(&payload->packet[2]));
	hdr.length = ntohs(get16Bit(&payload->packet[4]));
	if (hdr.length != payload->count) {
		free(payload->free_me);
		return 0;
	};
	hdr.checksum = get16Bit(&payload->packet[6]); // На little-endian машинах порядок байтов менять не нужно — он уже поменян при вычислении контрольной суммы
	struct ChecksumContext ctx;
	uint16_t computed_cs;
	if ((hdr.checksum == 0) || (
		set16Bit(&payload->packet[6], 0),
		initChecksum(&ctx),
		computeChecksum(&ctx, payload->pseudo, strategy->pseudo_length),
		computeChecksum(&ctx, payload->packet, payload->count),
		(computed_cs = getChecksum(&ctx)),
		((computed_cs == hdr.checksum) || ((hdr.checksum == 0xFFFF) && ((computed_cs == 0) || (computed_cs == 0xFFFF))))
		)) {
		// Проверка контрольной суммы прошла успешно. Нужно вставить в очередь на отправку
		strategy->port_setter(&addrs->src, hdr.src_port);
		strategy->port_setter(&addrs->dst, hdr.dst_port);
		struct UDPBinding *binding;
		binding = findUDPBinding(context, strategy, &addrs->src);
		if (binding == NULL) {
			free(payload->free_me);
			return 1;
		};
		struct UDPStackItem *item;
		item = malloc(sizeof(struct UDPStackItem));
		if (NULL == item) {
			pthread_mutex_unlock(&binding->mutex);
			free(payload->free_me);
			return 1;
		};
		item->send_me = payload->packet + 8;
		item->size = payload->count - 8;
		item->free_me = payload->free_me;
		item->dst = addrs->dst;
		item->next = binding->stack;
		binding->stack = item;
		if (-1 == event_add(binding->write_event, NULL)) {
			pthread_mutex_unlock(&binding->mutex);
			return 1;
		};
		pthread_mutex_unlock(&binding->mutex);
		return 0;
	} else {
		free(payload->free_me);
		return 0;
	};
};

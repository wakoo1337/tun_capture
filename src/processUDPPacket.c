#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <event2/event.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "CaptureContext.h"
#include "IPPacketPayload.h"
#include "SrcDstSockaddrs.h"
#include "UDPNetworkProtocolStrategy.h"
#include "UDPHeaderData.h"
#include "UDPQueueItem.h"
#include "ChecksumContext.h"
#include "UDPBinding.h"
#include "get16Bit.h"
#include "set16Bit.h"
#include "initChecksum.h"
#include "computeChecksum.h"
#include "getChecksum.h"
#include "findUDPBinding.h"
#include "udpCallback.h"

#include "processUDPPacket.h"
unsigned int processUDPPacket(struct CaptureContext *context, const struct IPPacketPayload *payload, const struct UDPNetworkProtocolStrategy *strategy, struct SrcDstSockaddrs *addrs) {
	struct UDPHeaderData hdr;
	if (payload->count < 8) return 1;
	hdr.src_port = ntohs(get16Bit(&payload->packet[0]));
	hdr.dst_port = ntohs(get16Bit(&payload->packet[2]));
	hdr.length = ntohs(get16Bit(&payload->packet[4]));
	if (hdr.length != payload->count) return 1;
	hdr.checksum = get16Bit(&payload->packet[6]); // На little-endian машинах порядок байтов менять не нужно -- он уже поменян при вычислении контрольной суммы
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
		binding = findUDPBinding(context, &addrs->src);
		if (binding == NULL) {
			return 1;
		};
		struct UDPQueueItem *queue_item;
		queue_item = malloc(sizeof(struct UDPQueueItem));
		if (NULL == queue_item) {
			pthread_mutex_unlock(&binding->mutex);
			return 1;
		};
		queue_item->send_me = payload->packet + 8;
		queue_item->size = payload->count - 8;
		queue_item->free_me = payload->free_me;
		queue_item->dst = addrs->dst;
		queue_item->next = binding->queue;
		binding->queue = queue_item;
		event_free(binding->sock_event);
		binding->sock_event = event_new(context->event_base, binding->sock, EV_READ | EV_WRITE | EV_PERSIST, &udpCallback, binding);
		event_add(binding->sock_event, NULL);
		pthread_mutex_unlock(&binding->mutex);
		return 0;
	};
	return 1;
};

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <event2/event.h>
#include "contrib/heap.h"
#include "ForwardingMappingIPv4.h"
#include "ForwardingMappingIPv6.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "UDPParameters.h"
#include "UDPBinding.h"
#include "ChecksumContext.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "RefcountBuffer.h"
#include "PacketQueueItem.h"
#include "initChecksum.h"
#include "computeChecksum.h"
#include "getChecksum.h"
#include "set16Bit.h"
#include "enqueueTxPacket.h"
#include "decrementRefcount.h"
#include "HEADERS_RESERVE.h"
#include "ipv4_strategy.h"
#include "ipv6_strategy.h"

#include "udpGenerator.h"
unsigned int udpGenerator(struct CaptureContext *context, struct RefcountBuffer *buffer, void *arg) {
	struct UDPParameters *parameters = (struct UDPParameters *) arg;
	const struct NetworkProtocolStrategy *strategy = (parameters->from.sa_family == AF_INET) ? &ipv4_strategy : ((parameters->from.sa_family == AF_INET6) ? &ipv6_strategy : NULL);
	assert(strategy);
	buffer->trapkt_offset = buffer->payload_offset - 8;
	const unsigned int udp_size = buffer->size - buffer->trapkt_offset;
	const unsigned int fragment_count = strategy->compute_fragcount(udp_size, context->settings->mtu);
	struct IPFragmentMetadata frag_metadata[fragment_count];
	set16Bit(&buffer->data[buffer->trapkt_offset+0], htons(strategy->port_getter(&parameters->from)));
	set16Bit(&buffer->data[buffer->trapkt_offset+2], htons(strategy->port_getter(&parameters->binding->internal_addr)));
	set16Bit(&buffer->data[buffer->trapkt_offset+4], htons(udp_size));
	set16Bit(&buffer->data[buffer->trapkt_offset+6], 0);
	uint8_t pseudo[strategy->pseudo_length];
	strategy->create_pseudo(pseudo, &parameters->from, &parameters->binding->internal_addr, 17, udp_size);
	struct ChecksumContext checksum_context;
	initChecksum(&checksum_context);
	computeChecksum(&checksum_context, pseudo, strategy->pseudo_length);
	computeChecksum(&checksum_context, &buffer->data[buffer->trapkt_offset+0], udp_size);
	uint16_t checksum;
	checksum = getChecksum(&checksum_context);
	set16Bit(&buffer->data[buffer->trapkt_offset+6], (checksum == 0) ? 0xFFFF : checksum);
	strategy->fill_metadatas(frag_metadata, fragment_count, udp_size, context->settings->mtu);
	if (fragment_count == 1) {
		buffer->netpkt_offset = buffer->trapkt_offset - frag_metadata->header_size;
		frag_metadata->buffer = &buffer->data[buffer->netpkt_offset];
		strategy->write_headers(context, frag_metadata, fragment_count, 17, &parameters->from, &parameters->binding->internal_addr);
		struct PacketQueueItem *queue_item;
		queue_item = malloc(sizeof(struct PacketQueueItem));
		if (NULL == queue_item) {
			decrementRefcount(&buffer);
			free(parameters);
			return 1;
		};
		queue_item->buffer = buffer;
		queue_item->processor = NULL;
		queue_item->arg = NULL;
		free(parameters);
		return enqueueTxPacket(context, queue_item);
	} else {
		free(parameters);
	};
	return 0;
};

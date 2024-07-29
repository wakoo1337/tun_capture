#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <alloca.h>
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
#include "PacketQueueItem.h"
#include "initChecksum.h"
#include "computeChecksum.h"
#include "getChecksum.h"
#include "set16Bit.h"
#include "enqueueTxPacket.h"
#include "HEADERS_RESERVE.h"
#include "ipv4_strategy.h"
#include "ipv6_strategy.h"

#include "udpGenerator.h"
unsigned int udpGenerator(struct CaptureContext *context, uint8_t *packet, unsigned int size, void *arg) {
	struct UDPParameters *parameters = (struct UDPParameters *) arg;
	const struct NetworkProtocolStrategy *strategy = (parameters->from.sa_family == AF_INET) ? &ipv4_strategy : ((parameters->from.sa_family == AF_INET6) ? &ipv6_strategy : NULL);
	if (NULL == strategy) return 1;
	const unsigned int fragment_count = strategy->compute_fragcount(size, context->settings->mtu);
	struct IPFragmentMetadata *frag_metadata;
	frag_metadata = alloca(fragment_count * sizeof(struct IPFragmentMetadata));
	uint8_t *udp_header;
	udp_header = &packet[-8];
	set16Bit(&udp_header[0], htons(strategy->port_getter(&parameters->from)));
	set16Bit(&udp_header[2], htons(strategy->port_getter(&parameters->binding->internal_addr)));
	set16Bit(&udp_header[4], htons(size + 8));
	set16Bit(&udp_header[6], 0);
	uint8_t *pseudo;
	pseudo = alloca(strategy->pseudo_length);
	strategy->create_pseudo(pseudo, &parameters->from, &parameters->binding->internal_addr, 17, size + 8);
	struct ChecksumContext checksum_context;
	initChecksum(&checksum_context);
	computeChecksum(&checksum_context, pseudo, strategy->pseudo_length);
	computeChecksum(&checksum_context, udp_header, size + 8);
	uint16_t checksum;
	checksum = getChecksum(&checksum_context);
	set16Bit(&udp_header[6], (checksum == 0) ? 0xFFFF : checksum);
	strategy->fill_metadatas(frag_metadata, fragment_count, size + 8, context->settings->mtu);
	if (fragment_count == 1) {
		frag_metadata->buffer = &packet[(signed int) -(frag_metadata->header_size+8)];
		strategy->write_headers(context, frag_metadata, fragment_count, 17, &parameters->from, &parameters->binding->internal_addr);
		struct PacketQueueItem *queue_item;
		queue_item = malloc(sizeof(struct PacketQueueItem));
		if (NULL == queue_item) {
			free(parameters);
			return 1;
		};
		queue_item->data = frag_metadata->buffer;
		queue_item->count = frag_metadata->header_size + frag_metadata->data_size;
		queue_item->processor = NULL;
		queue_item->free_me = &packet[-HEADERS_RESERVE];
		queue_item->arg = NULL;
		free(parameters);
		return enqueueTxPacket(context, queue_item);
	} else {
		free(parameters);
	};
	return 0;
};

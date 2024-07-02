#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <alloca.h>
#include <string.h>
#include <event2/event.h>
#include "contrib/C-Collections/pqlib/PQ.h"
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
#include "tunCallback.h"
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
	uint16_t value;
	udp_header = &packet[-8];
	value = htons(strategy->port_getter(&parameters->from));
	memcpy(&udp_header[0], &value, sizeof(uint16_t));
	value = htons(strategy->port_getter(&parameters->binding->internal_addr));
	memcpy(&udp_header[2], &value, sizeof(uint16_t));
	value = htons(size + 8);
	memcpy(&udp_header[4], &value, sizeof(uint16_t));
	value = 0;
	memcpy(&udp_header[6], &value, sizeof(uint16_t));
	uint8_t *pseudo;
	pseudo = alloca(strategy->pseudo_length);
	strategy->create_pseudo(pseudo, &parameters->from, &parameters->binding->internal_addr, 17, size + 8);
	struct ChecksumContext checksum;
	initChecksum(&checksum);
	computeChecksum(&checksum, pseudo, strategy->pseudo_length);
	computeChecksum(&checksum, udp_header, size + 8);
	value = getChecksum(&checksum);
	if (value == 0) value = 0xFFFF;
	memcpy(&udp_header[6], &value, sizeof(uint16_t));
	strategy->fill_metadatas(frag_metadata, fragment_count, size + 8, context->settings->mtu);
	if (fragment_count == 1) {
		frag_metadata->buffer = &packet[(signed int) -(frag_metadata->header_size+8)];
		strategy->write_headers(context, frag_metadata, fragment_count, 17, &parameters->from, &parameters->binding->internal_addr);
		struct PacketQueueItem *queue_item;
		queue_item = malloc(sizeof(struct PacketQueueItem));
		if (NULL == queue_item) return 1;
		queue_item->data = frag_metadata->buffer;
		queue_item->count = frag_metadata->header_size + frag_metadata->data_size;
		queue_item->processor = NULL;
		queue_item->free_me = &packet[-HEADERS_RESERVE];
		queue_item->arg = NULL;
		pthread_mutex_lock(&context->queue_mutex);
		queue_item->next = context->send_stack;
		context->send_stack = queue_item;
		event_free(context->iface_event);
		context->iface_event = event_new(context->event_base, context->settings->fd_getter(context->settings->user), EV_READ | EV_WRITE | EV_PERSIST, &tunCallback, context);
		if (NULL == context->iface_event) {
			return 1;
		};
		if (-1 == event_add(context->iface_event, NULL)) {
			return 1;
		};
		pthread_mutex_unlock(&context->queue_mutex);
	} else {
	};
	free(parameters);
	return 0;
};

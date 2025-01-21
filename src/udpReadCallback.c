#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "contrib/logdel_heap.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "UDPParameters.h"
#include "UDPStackItem.h"
#include "UDPBinding.h"
#include "udpGenerator.h"
#include "emergencyStop.h"
#include "enqueueRxPacket.h"
#include "HEADERS_RESERVE.h"
#include "MAX_UDP_PAYLOAD.h"

#include "udpReadCallback.h"
void udpReadCallback(evutil_socket_t fd, short what, void *arg) {
	struct UDPBinding *binding;
	binding = (struct UDPBinding *) arg;
	assert(((int) fd) == binding->sock);
	if (what & EV_READ) {
		uint8_t *buffer;
		buffer = malloc((HEADERS_RESERVE + MAX_UDP_PAYLOAD) * sizeof(uint8_t));
		if (NULL == buffer) {
			emergencyStop(binding->context);
			return;
		};
		struct sockaddr sender_sa = {0};
		socklen_t sl = sizeof(struct sockaddr);
		ssize_t result;
		result = recvfrom(binding->sock, &buffer[HEADERS_RESERVE], MAX_UDP_PAYLOAD, 0, &sender_sa, &sl);
		if (result == -1) {
			free(buffer);
			if ((errno != EAGAIN) && (errno != EWOULDBLOCK) && (errno != ECONNREFUSED) && (errno != ECONNABORTED) && (errno != EHOSTUNREACH) && (errno != EPIPE)) emergencyStop(binding->context);
			return;
		};
		assert(sender_sa.sa_family == binding->internal_addr.sa_family);
		uint8_t *new_buffer;
		new_buffer = realloc(buffer, (HEADERS_RESERVE + result) * sizeof(uint8_t));
		if (NULL == new_buffer) {
			free(buffer);
			emergencyStop(binding->context);
			return;
		};
		struct PacketQueueItem *queue_item;
		queue_item = malloc(sizeof(struct PacketQueueItem));
		if (NULL == queue_item) {
			free(new_buffer);
			emergencyStop(binding->context);
			return;
		};
		queue_item->data = &new_buffer[HEADERS_RESERVE];
		queue_item->free_me = new_buffer;
		queue_item->count = result;
		queue_item->processor = &udpGenerator;
		queue_item->mutex = NULL;
		struct UDPParameters *params;
		params = malloc(sizeof(struct UDPParameters));
		if (NULL == params) {
			free(new_buffer);
			free(queue_item);
			emergencyStop(binding->context);
			return;
		};
		params->from = sender_sa;
		params->binding = binding;
		queue_item->arg = params;
		enqueueRxPacket(binding->context, queue_item);
	};
};

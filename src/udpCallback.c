#include <event2/event.h>
#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include "contrib/heap.h"
#include "UDPQueueItem.h"
#include "UDPBinding.h"
#include "UDPParameters.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "HEADERS_RESERVE.h"
#include "MAX_UDP_PAYLOAD.h"
#include "udpGenerator.h"
#include "enqueuePacket.h"

#include "udpCallback.h"
void udpCallback(evutil_socket_t fd, short what, void *arg) {
	struct UDPBinding *binding;
	binding = (struct UDPBinding *) arg;
	assert(((int) fd) == binding->sock);
	if (what & EV_READ) {
		uint8_t *buffer;
		buffer = malloc((HEADERS_RESERVE + MAX_UDP_PAYLOAD) * sizeof(uint8_t));
		if (NULL == buffer) return;
		struct sockaddr sender_sa = {0};
		socklen_t sl = sizeof(struct sockaddr);
		ssize_t result;
		result = recvfrom(binding->sock, &buffer[HEADERS_RESERVE], MAX_UDP_PAYLOAD, 0, &sender_sa, &sl);
		if (result == -1) {
			free(buffer);
			return;
		};
		assert(sender_sa.sa_family == binding->internal_addr.sa_family);
		buffer = realloc(buffer, (HEADERS_RESERVE + result) * sizeof(uint8_t));
		if (NULL == buffer) return;
		struct PacketQueueItem *queue_item;
		queue_item = malloc(sizeof(struct PacketQueueItem));
		if (NULL == queue_item) {
			free(buffer);
			return;
		};
		queue_item->data = &buffer[HEADERS_RESERVE];
		queue_item->count = result;
		queue_item->processor = &udpGenerator;
		queue_item->mutex = NULL;
		struct UDPParameters *params;
		params = malloc(sizeof(struct UDPParameters));
		if (NULL == params) {
			free(buffer);
			free(queue_item);
			return;
		};
		params->from = sender_sa;
		params->binding = binding;
		queue_item->arg = params;
		pthread_mutex_lock(&binding->context->queue_mutex);
		enqueuePacket(binding->context, queue_item);
		pthread_mutex_unlock(&binding->context->queue_mutex);
		pthread_cond_signal(&binding->context->queue_cond);
	};
	if (what & EV_WRITE) {
		pthread_mutex_lock(&binding->mutex);
		while (binding->queue) {
			struct UDPQueueItem *next;
			next = binding->queue->next;
			ssize_t result;
			result = sendto(binding->sock, binding->queue->send_me, binding->queue->size, 0, &binding->queue->dst, sizeof(struct sockaddr));
			if (-1 == result) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					pthread_mutex_unlock(&binding->mutex);
					return;
				} else {
					free(binding->queue->free_me);
					free(binding->queue);
					binding->queue = next;
				};
			} else {
				free(binding->queue->free_me);
				free(binding->queue);
				binding->queue = next;
			};
		};
		if (NULL == binding->queue) {
			event_free(binding->sock_event);
			binding->sock_event = event_new(binding->context->event_base, binding->sock, EV_READ | EV_PERSIST, &udpCallback, binding);
			event_add(binding->sock_event, NULL);
		};
		pthread_mutex_unlock(&binding->mutex);
	};
};

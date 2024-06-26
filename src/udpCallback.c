#include <event2/event.h>
#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "UDPQueueItem.h"
#include "UDPBinding.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "HEADERS_RESERVE.h"
#include "MAX_UDP_PAYLOAD.h"
#include "udpProcessor.h"

#include "udpCallback.h"
void udpCallback(evutil_socket_t fd, short what, void *arg) {
	struct UDPBinding *binding;
	binding = (struct UDPBinding *) arg;
	assert(((int) fd) == binding->sock);
	if (what & EV_READ) {
		uint8_t *buffer;
		buffer = malloc((HEADERS_RESERVE + MAX_UDP_PAYLOAD) * sizeof(uint8_t));
		struct sockaddr *sender_sa;
		sender_sa = malloc(sizeof(struct sockaddr));
		if ((NULL == buffer) || (NULL == sender_sa)) {
			free(buffer);
			free(sender_sa);
			return;
		};
		*sender_sa = (struct sockaddr) {0};
		socklen_t sl = sizeof(struct sockaddr);
		ssize_t result;
		result = recvfrom(binding->sock, &buffer[HEADERS_RESERVE], MAX_UDP_PAYLOAD, 0, sender_sa, &sl);
		if (result == -1) {
			free(buffer);
			free(sender_sa);
			return;
		};
		struct PacketQueueItem *queue_item;
		queue_item = malloc(sizeof(struct PacketQueueItem));
		if (NULL == queue_item) {
			free(buffer);
			free(sender_sa);
			return;
		};
		queue_item->data = &buffer[HEADERS_RESERVE];
		queue_item->count = result;
		queue_item->processor = &udpProcessor;
		queue_item->arg = sender_sa;
		pthread_mutex_lock(&binding->context->queue_mutex);
		queue_item->next = binding->context->captured_stack;
		binding->context->captured_stack = queue_item;
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
		pthread_mutex_unlock(&binding->mutex);
	};
};

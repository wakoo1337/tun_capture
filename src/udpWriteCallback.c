#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "UDPBinding.h"
#include "RefcountBuffer.h"
#include "UDPStackItem.h"
#include "emergencyStop.h"
#include "decrementRefcount.h"

#include "udpWriteCallback.h"
void udpWriteCallback(evutil_socket_t fd, short what, void *arg) {
	struct UDPBinding *binding;
	binding = (struct UDPBinding *) arg;
	if (what & EV_WRITE) {
		pthread_mutex_lock(&binding->mutex);
		while (binding->stack) {
			struct UDPStackItem *current, *next;
			current = binding->stack;
			next = current->next;
			unsigned int payload_offset, payload_size;
			pthread_mutex_lock(&current->buffer->mutex);
			payload_offset = current->buffer->payload_offset;
			payload_size = current->buffer->size - payload_offset;
			pthread_mutex_unlock(&current->buffer->mutex);
			ssize_t result;
			result = sendto(binding->sock, &current->buffer->data[payload_offset], payload_size, 0, &binding->stack->dst, sizeof(struct sockaddr));
			if (-1 == result) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					pthread_mutex_unlock(&binding->mutex);
					return;
				} else {
					decrementRefcount(&current->buffer);
					free(current);
					binding->stack = next;
				};
			} else {
				decrementRefcount(&current->buffer);
				free(current);
				binding->stack = next;
			};
		};
		if ((NULL == binding->stack) && (-1 == event_del(binding->write_event))) {
			pthread_mutex_unlock(&binding->mutex);
			emergencyStop(binding->context);
			return;
		};
		pthread_mutex_unlock(&binding->mutex);
	};
};

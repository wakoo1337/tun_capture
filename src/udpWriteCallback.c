#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "UDPBinding.h"
#include "UDPStackItem.h"
#include "emergencyStop.h"

#include "udpWriteCallback.h"
void udpWriteCallback(evutil_socket_t fd, short what, void *arg) {
	struct UDPBinding *binding;
	binding = (struct UDPBinding *) arg;
	if (what & EV_WRITE) {
		pthread_mutex_lock(&binding->mutex);
		while (binding->stack) {
			struct UDPStackItem *next;
			next = binding->stack->next;
			ssize_t result;
			result = sendto(binding->sock, binding->stack->send_me, binding->stack->size, 0, &binding->stack->dst, sizeof(struct sockaddr));
			if (-1 == result) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					pthread_mutex_unlock(&binding->mutex);
					return;
				} else {
					free(binding->stack->free_me);
					free(binding->stack);
					binding->stack = next;
				};
			} else {
				free(binding->stack->free_me);
				free(binding->stack);
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

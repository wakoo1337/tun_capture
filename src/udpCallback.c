#include <event2/event.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/socket.h>
#include <errno.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "UDPQueueItem.h"
#include "UDPBinding.h"

#include "udpCallback.h"
void udpCallback(evutil_socket_t fd, short what, void *arg) {
	struct UDPBinding *binding;
	binding = (struct UDPBinding *) arg;
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

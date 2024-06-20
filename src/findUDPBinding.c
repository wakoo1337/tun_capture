#include <sys/socket.h>
#include <pthread.h>
#include <event2/event.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "contrib/avl.h"
#include "contrib/C-Collections/pqlib/PQ.h"
#include "CaptureContext.h"
#include "UDPBinding.h"
#include "udpCallback.h"

#include "findUDPBinding.h"
struct UDPBinding *findUDPBinding(struct CaptureContext *context, const struct sockaddr *sa) {
	pthread_mutex_lock(&context->udp_mutex);
	struct UDPBinding *binding;
	binding = avl_find(context->udp_bindings, &sa);
	if (binding) {
		pthread_mutex_lock(&binding->mutex);
		pthread_mutex_unlock(&context->udp_mutex);
		return binding;
	} else {
		binding = malloc(sizeof(struct UDPBinding));
		if (NULL == binding) {
			pthread_mutex_unlock(&context->udp_mutex);
			return NULL;
		};
		binding->context = context;
		binding->internal_addr = *sa;
		binding->sock = socket(sa->sa_family, SOCK_DGRAM, 0);
		if (-1 == binding->sock) {
			pthread_mutex_unlock(&context->udp_mutex);
			free(binding);
			return NULL;
		};
		struct sockaddr bind_addr = {0}; // Это будет любой адрес
		((struct sockaddr_in *) &bind_addr)->sin_family = sa->sa_family;
		if (-1 == bind(binding->sock, &bind_addr, sizeof(struct sockaddr))) {
			pthread_mutex_unlock(&context->udp_mutex);
			close(binding->sock);
			free(binding);
			return NULL;
		};
		if (-1 == fcntl(binding->sock, F_SETFL, O_NONBLOCK)) {
			pthread_mutex_unlock(&context->udp_mutex);
			close(binding->sock);
			free(binding);
			return NULL;
		};
		binding->sock_event = event_new(context->event_base, binding->sock, EV_READ | EV_PERSIST, &udpCallback, binding);
		if (NULL == binding->sock_event) {
			pthread_mutex_unlock(&context->udp_mutex);
			close(binding->sock);
			free(binding);
			return NULL;
		};
		event_add(binding->sock_event, NULL);
		binding->queue = NULL;
		void **probe;
		probe = avl_probe(context->udp_bindings, binding);
		if ((NULL == probe) || ((*probe) != binding)) {
			pthread_mutex_unlock(&context->udp_mutex);
			close(binding->sock);
			free(binding);
			return NULL;
		};
		pthread_mutex_init(&binding->mutex, NULL);
		pthread_mutex_lock(&binding->mutex);
		pthread_mutex_unlock(&context->udp_mutex);
		return binding;
	};
};

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "contrib/avl.h"
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "UDPBinding.h"
#include "udpReadCallback.h"
#include "udpWriteCallback.h"

#include "findUDPBinding.h"
struct UDPBinding *findUDPBinding(struct CaptureContext *context, const struct sockaddr *sa) {
	pthread_mutex_lock(&context->udp_mutex);
	struct UDPBinding *binding;
	binding = avl_find(context->udp_bindings, sa);
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
		pthread_mutex_init(&binding->mutex, NULL);
		pthread_mutex_lock(&binding->mutex);
		binding->context = context;
		binding->internal_addr = *sa;
		binding->sock = socket(sa->sa_family, SOCK_DGRAM, 0);
		if (-1 == binding->sock) {
			pthread_mutex_unlock(&binding->mutex);
			pthread_mutex_unlock(&context->udp_mutex);
			free(binding);
			return NULL;
		};
		struct sockaddr bind_addr = {0}; // Это будет любой адрес
		bind_addr.sa_family = sa->sa_family;
		if (-1 == bind(binding->sock, &bind_addr, sizeof(struct sockaddr))) { // TODO не привязываться, если порт равен 0
			pthread_mutex_unlock(&binding->mutex);
			pthread_mutex_unlock(&context->udp_mutex);
			close(binding->sock);
			free(binding);
			return NULL;
		};
		if (-1 == fcntl(binding->sock, F_SETFL, O_NONBLOCK)) {
			pthread_mutex_unlock(&binding->mutex);
			pthread_mutex_unlock(&context->udp_mutex);
			close(binding->sock);
			free(binding);
			return NULL;
		};
		binding->read_event = event_new(context->event_base, binding->sock, EV_READ | EV_PERSIST | EV_FINALIZE, &udpReadCallback, binding);
		if (NULL == binding->read_event) {
			pthread_mutex_unlock(&binding->mutex);
			pthread_mutex_unlock(&context->udp_mutex);
			close(binding->sock);
			free(binding);
			return NULL;
		};
		binding->write_event = event_new(context->event_base, binding->sock, EV_WRITE | EV_PERSIST | EV_FINALIZE, &udpWriteCallback, binding);
		if (NULL == binding->write_event) {
			pthread_mutex_unlock(&binding->mutex);
			event_free(binding->read_event);
			pthread_mutex_unlock(&context->udp_mutex);
			close(binding->sock);
			free(binding);
			return NULL;
		};
		if (-1 == event_add(binding->read_event, NULL)) {
			pthread_mutex_unlock(&binding->mutex);
			event_free(binding->read_event);
			event_free(binding->write_event);
			pthread_mutex_unlock(&context->udp_mutex);
			close(binding->sock);
			free(binding);
			return NULL;
		};
		binding->stack = NULL;
		void **probe;
		probe = avl_probe(context->udp_bindings, binding);
		if ((NULL == probe) || ((*probe) != binding)) {
			pthread_mutex_unlock(&binding->mutex);
			event_free(binding->read_event);
			event_free(binding->write_event);
			pthread_mutex_unlock(&context->udp_mutex);
			close(binding->sock);
			free(binding);
			return NULL;
		};
		pthread_mutex_unlock(&context->udp_mutex);
		return binding;
	};
};

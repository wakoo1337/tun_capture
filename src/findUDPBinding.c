#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "contrib/avl.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "UDPBinding.h"
#include "udpReadCallback.h"
#include "udpWriteCallback.h"
#include "udpFinalizeRead.h"
#include "udpFinalizeWrite.h"
#include "getMonotonicTimeval.h"
#include "addTimeval.h"
#include "enqueueTimeout.h"
#include "udpTimeoutCallback.h"
#include "udp_timeout.h"

#include "findUDPBinding.h"
struct UDPBinding *findUDPBinding(struct CaptureContext *context, const struct NetworkProtocolStrategy *strategy, const struct sockaddr *sa) {
	pthread_mutex_lock(&context->udp_mutex);
	struct UDPBinding *binding;
	binding = avl_find(context->udp_bindings, sa);
	if (binding) {
		sem_wait(&binding->semaphore);
		pthread_mutex_lock(&binding->mutex);
		pthread_mutex_unlock(&context->udp_mutex);
		return binding;
	} else {
		binding = malloc(sizeof(struct UDPBinding));
		if (NULL == binding) {
			pthread_mutex_unlock(&context->udp_mutex);
			return NULL;
		};
		sem_init(&binding->semaphore, 0, context->settings->threads_count);
		sem_wait(&binding->semaphore);
		pthread_mutex_init(&binding->mutex, NULL);
		pthread_mutex_lock(&binding->mutex);
		binding->context = context;
		binding->internal_addr = *sa;
		binding->stack = NULL;
		binding->timeout = NULL;
		binding->read_alive = binding->write_alive = true;
		binding->read_event = binding->write_event = NULL;
		binding->persistent = false;
		void **probe;
		probe = avl_probe(context->udp_bindings, binding);
		if ((NULL == probe) || ((*probe) != binding)) {
			pthread_mutex_unlock(&binding->mutex);
			pthread_mutex_destroy(&binding->mutex);
			sem_post(&binding->semaphore);
			sem_destroy(&binding->semaphore);
			pthread_mutex_unlock(&context->udp_mutex);
			free(binding);
			return NULL;
		};
		binding->sock = socket(sa->sa_family, SOCK_DGRAM, 0);
		if (-1 == binding->sock) {
			void *deleted;
			deleted = avl_delete(context->udp_bindings, binding);
			assert(deleted == binding);
			pthread_mutex_unlock(&binding->mutex);
			pthread_mutex_destroy(&binding->mutex);
			sem_post(&binding->semaphore);
			sem_destroy(&binding->semaphore);
			pthread_mutex_unlock(&context->udp_mutex);
			free(binding);
			return NULL;
		};
		if (strategy->port_getter(sa)) {
			struct sockaddr bind_addr = {0}; // Это будет любой адрес
			bind_addr.sa_family = sa->sa_family;
			if (-1 == bind(binding->sock, &bind_addr, sizeof(struct sockaddr))) {
				void *deleted;
				deleted = avl_delete(context->udp_bindings, binding);
				assert(deleted == binding);
				pthread_mutex_unlock(&binding->mutex);
				pthread_mutex_destroy(&binding->mutex);
				sem_post(&binding->semaphore);
				sem_destroy(&binding->semaphore);
				pthread_mutex_unlock(&context->udp_mutex);
				close(binding->sock);
				free(binding);
				return NULL;
			};
		};
		if (-1 == fcntl(binding->sock, F_SETFL, O_NONBLOCK)) {
			void *deleted;
			deleted = avl_delete(context->udp_bindings, binding);
			assert(deleted == binding);
			pthread_mutex_unlock(&binding->mutex);
			pthread_mutex_destroy(&binding->mutex);
			sem_post(&binding->semaphore);
			sem_destroy(&binding->semaphore);
			pthread_mutex_unlock(&context->udp_mutex);
			close(binding->sock);
			free(binding);
			return NULL;
		};
		binding->read_event = event_new(context->event_base, binding->sock, EV_READ | EV_PERSIST | EV_FINALIZE, &udpReadCallback, binding);
		if (NULL == binding->read_event) {
			void *deleted;
			deleted = avl_delete(context->udp_bindings, binding);
			assert(deleted == binding);
			pthread_mutex_unlock(&binding->mutex);
			pthread_mutex_destroy(&binding->mutex);
			sem_post(&binding->semaphore);
			sem_destroy(&binding->semaphore);
			pthread_mutex_unlock(&context->udp_mutex);
			close(binding->sock);
			free(binding);
			return NULL;
		};
		binding->write_event = event_new(context->event_base, binding->sock, EV_WRITE | EV_PERSIST | EV_FINALIZE, &udpWriteCallback, binding);
		if (NULL == binding->write_event) {
			udpFinalizeRead(binding);
			pthread_mutex_unlock(&binding->mutex);
			sem_post(&binding->semaphore);
			pthread_mutex_unlock(&context->udp_mutex);
			return NULL;
		};
		if (-1 == event_add(binding->read_event, NULL)) {
			udpFinalizeRead(binding);
			udpFinalizeWrite(binding);
			pthread_mutex_unlock(&binding->mutex);
			sem_post(&binding->semaphore);
			pthread_mutex_unlock(&context->udp_mutex);
			return NULL;
		};
		pthread_mutex_unlock(&binding->mutex);
		pthread_mutex_lock(&context->timeout_mutex);
		pthread_mutex_lock(&binding->mutex);
		struct timeval now, timeout;
		getMonotonicTimeval(&now);
		addTimeval(&now, &udp_timeout, &timeout);
		binding->timeout = enqueueTimeout(context, &timeout, &udpTimeoutCallback, binding);
		if (NULL == binding->timeout) {
			udpFinalizeRead(binding);
			udpFinalizeWrite(binding);
			pthread_mutex_unlock(&binding->mutex);
			pthread_mutex_unlock(&context->timeout_mutex);
			sem_post(&binding->semaphore);
			pthread_mutex_unlock(&context->udp_mutex);
			return NULL;
		};
		pthread_mutex_unlock(&context->timeout_mutex);
		pthread_mutex_unlock(&context->udp_mutex);
		return binding;
	};
};

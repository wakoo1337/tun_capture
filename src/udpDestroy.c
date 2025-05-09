#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "contrib/avl.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "PacketQueueItem.h"
#include "UDPStackItem.h"
#include "TimeoutItem.h"
#include "UDPBinding.h"
#include "udpGenerator.h"

#include "udpDestroy.h"
void udpDestroy(struct UDPBinding *binding) {
	pthread_mutex_unlock(&binding->mutex);
	pthread_mutex_lock(&binding->context->udp_mutex);
	pthread_mutex_lock(&binding->mutex);
	void *deleted;
	deleted = avl_delete(binding->context->udp_bindings, binding);
	assert(deleted == binding);
	pthread_mutex_unlock(&binding->context->udp_mutex);
	pthread_mutex_unlock(&binding->mutex);
	pthread_mutex_lock(&binding->context->rx_mutex);
	struct PacketQueueItem *rx_current, **rx_prevnext;
	rx_current = binding->context->rx_begin;
	rx_prevnext = &binding->context->rx_begin;
	while (rx_current) {
		if (rx_current->processor == &udpGenerator) {
			struct PacketQueueItem *next;
			next = rx_current->next;
			free(rx_current->arg);
			free(rx_current->free_me);
			free(rx_current);
			*rx_prevnext = next;
			rx_current = next;
		} else {
			rx_prevnext = &rx_current->next;
			rx_current = *rx_prevnext;
		};
	};
	if (NULL == binding->context->rx_begin) binding->context->rx_end = &binding->context->rx_begin;
	pthread_mutex_unlock(&binding->context->rx_mutex);
	for (unsigned int i=0;i < binding->context->settings->threads_count;i++) sem_wait(&binding->semaphore);
	pthread_mutex_lock(&binding->mutex);
	while (binding->stack) {
		struct UDPStackItem *next;
		next = binding->stack->next;
		free(binding->stack->free_me);
		free(binding->stack);
		binding->stack = next;
	};
	pthread_mutex_unlock(&binding->mutex);
	pthread_mutex_destroy(&binding->mutex);
	for (unsigned int i=0;i < binding->context->settings->threads_count;i++) sem_post(&binding->semaphore);
	sem_destroy(&binding->semaphore);
	close(binding->sock);
	free(binding);
};

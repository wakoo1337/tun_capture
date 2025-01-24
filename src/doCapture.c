#include <pthread.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <event2/event.h>
#include <event2/thread.h>
#include "contrib/avl.h"
#include "contrib/logdel_heap.h"
#include "ForwardingMappingIPv4.h"
#include "ForwardingMappingIPv6.h"
#include "CaptureContext.h"
#include "CaptureSettings.h"
#include "UDPBinding.h"
#include "tunReadCallback.h"
#include "tunWriteCallback.h"
#include "threadWorker.h"
#include "compareIPv4FragmentsIdsSources.h"
#include "compareTimeoutItems.h"
#include "compareUDPBindings.h"
#include "timerCallback.h"
#include "compareTCPConnections.h"

#include "doCapture.h"
unsigned int doCapture(const struct CaptureSettings *settings) {
	struct CaptureContext *context;
	context = malloc(sizeof(struct CaptureContext));
	if (NULL == context) return 1;
	context->settings = settings;
	context->ipv4_id = 0;
	context->ipv6_id = 0;
	pthread_mutex_init(&context->rx_mutex, NULL);
	pthread_cond_init(&context->rx_cond, NULL);
	pthread_mutex_init(&context->tx_mutex, NULL);
	context->running = true;
	pthread_mutex_init(&context->timeout_mutex, NULL);
	context->rx_begin = NULL;
	context->rx_end = &context->rx_begin;
	context->tx_begin = NULL;
	context->tx_end = &context->tx_begin;
	context->ipv4_fragments = avl_create(&compareIPv4FragmentsIdsSources, NULL, NULL);
	context->timeout_queue = logdelheap_create(&compareTimeoutItems, NULL);
	if (evthread_use_pthreads()) {
		free(context);
		return 1;
	};
	context->event_base = event_base_new();
	if (NULL == context->event_base) {
		free(context);
		return 1;
	};
	context->rx_event = event_new(context->event_base, settings->fd_getter(settings->user), EV_READ | EV_PERSIST, &tunReadCallback, context);
	if (NULL == context->rx_event) {
		event_base_free(context->event_base);
		free(context);
		return 1;
	};
	context->tx_event = event_new(context->event_base, settings->fd_getter(settings->user), EV_WRITE | EV_PERSIST | EV_FINALIZE, &tunWriteCallback, context);
	if (NULL == context->tx_event) {
		event_free(context->rx_event);
		event_base_free(context->event_base);
		free(context);
		return 1;
	};
	if (-1 == event_add(context->rx_event, NULL)) {
		event_free(context->rx_event);
		event_free(context->tx_event);
		event_base_free(context->event_base);
		free(context);
		return 1;
	};
	context->timeout_event = event_new(context->event_base, -1, 0, &timerCallback, context);
	if (NULL == context->timeout_event) {
		event_free(context->rx_event);
		event_free(context->tx_event);
		event_base_free(context->event_base);
		free(context);
		return 1;
	};
	pthread_mutex_init(&context->udp_mutex, NULL);
	context->udp_bindings = avl_create(&compareUDPBindings, NULL, NULL);
	pthread_mutex_init(&context->tcp_mutex, NULL);
	context->tcp_connections = avl_create(&compareTCPConnections, NULL, NULL);
	context->threads = malloc(context->settings->threads_count * sizeof(pthread_t));
	if (NULL == context->threads) {
		event_free(context->rx_event);
		event_free(context->tx_event);
		event_free(context->timeout_event);
		event_base_free(context->event_base);
		free(context);
		return 1;
	};
	pthread_mutex_init(&context->ipv4_id_mutex, NULL);
	pthread_mutex_init(&context->ipv6_id_mutex, NULL);
	for (unsigned int i=0;i < context->settings->threads_count;i++) {
		if (pthread_create(&context->threads[i], NULL, &threadWorker, context)) {
			free(context->threads);
			free(context);
			return 1;
		};
	};
	int loop_result;
	loop_result = event_base_loop(context->event_base, EVLOOP_NO_EXIT_ON_EMPTY);
	event_base_free(context->event_base);
	free(context);
	return 0;
};

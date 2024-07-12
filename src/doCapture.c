#include <pthread.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <event2/event.h>
#include "contrib/avl.h"
#include "contrib/heap.h"
#include "ForwardingMappingIPv4.h"
#include "ForwardingMappingIPv6.h"
#include "CaptureContext.h"
#include "CaptureSettings.h"
#include "tunCallback.h"
#include "threadWorker.h"
#include "compareIPv4FragmentsIdsSources.h"
#include "compareTimeoutItems.h"
#include "compareUDPBindings.h"
#include "UDPBinding.h"
#include "udpCallback.h"
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
	pthread_mutex_init(&context->queue_mutex, NULL);
	pthread_mutex_init(&context->timeout_mutex, NULL);
	pthread_cond_init(&context->queue_cond, NULL);
	context->captured_stack = NULL;
	context->send_stack = NULL;
	context->ipv4_fragments = avl_create(&compareIPv4FragmentsIdsSources, NULL, NULL);
	context->timeout_queue = heap_new(&compareTimeoutItems, NULL);
	context->threads = malloc(context->settings->threads_count * sizeof(pthread_t));
	for (unsigned int i=0;i < context->settings->threads_count;i++) {
		if (pthread_create(&context->threads[i], NULL, &threadWorker, context)) {
			free(context->threads);
			free(context);
			return 1;
		};
	};
	context->event_base = event_base_new();
	if (NULL == context->event_base) {
		free(context->threads);
		free(context);
		return 1;
	};
	context->iface_event = event_new(context->event_base, settings->fd_getter(settings->user), EV_READ | EV_PERSIST, &tunCallback, context);
	if (NULL == context->iface_event) {
		event_base_free(context->event_base);
		free(context->threads);
		free(context);
		return 1;
	};
	if (-1 == event_add(context->iface_event, NULL)) {
		event_free(context->iface_event);
		event_base_free(context->event_base);
		free(context->threads);
		free(context);
		return 1;
	};
	context->timeout_event = event_new(context->event_base, -1, 0, &timerCallback, context);
	if (NULL == context->timeout_event) {
		event_free(context->iface_event);
		event_base_free(context->event_base);
		free(context->threads);
		free(context);
		return 1;
	};
	pthread_mutex_init(&context->udp_mutex, NULL);
	context->udp_bindings = avl_create(&compareUDPBindings, NULL, NULL);
	pthread_mutex_init(&context->tcp_mutex, NULL);
	context->tcp_connections = avl_create(&compareTCPConnections, NULL, NULL);
	int loop_result;
	loop_result = event_base_loop(context->event_base, EVLOOP_NO_EXIT_ON_EMPTY);
	event_base_free(context->event_base);
	free(context);
	return 0;
};

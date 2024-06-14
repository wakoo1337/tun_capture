#include <pthread.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <event2/event.h>
#include "CaptureContext.h"
#include "CaptureSettings.h"
#include "tunCallback.h"
#include "threadWorker.h"

#include "doCapture.h"
unsigned int doCapture(struct CaptureSettings *settings) {
	struct CaptureContext *context;
	context = malloc(sizeof(struct CaptureContext));
	if (NULL == context) return 1;
	context->settings = settings;
	pthread_mutex_init(&context->queue_mutex, NULL);
	pthread_cond_init(&context->queue_cond, NULL);
	context->captured_stack = NULL;
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
	context->iface_event = event_new(context->event_base, settings->fd, EV_READ | EV_PERSIST, &tunCallback, context);
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
	int loop_result;
	loop_result = event_base_loop(context->event_base, EVLOOP_NO_EXIT_ON_EMPTY);
	event_base_free(context->event_base);
	free(context);
	return 0;

};

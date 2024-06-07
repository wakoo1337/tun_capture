#include <stdbool.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <event2/event.h>
#include "CaptureContext.h"
#include "CaptureSettings.h"
#include "tunCallback.h"

#include "doCapture.h"
unsigned int doCapture(struct CaptureSettings *settings) {
	settings->context = malloc(sizeof(struct CaptureContext));
	if (NULL == settings->context) return 1;
	struct CaptureContext *context = settings->context;
	context->event_base = event_base_new();
	if (NULL == context->event_base) {
		free(context);
		return 1;
	};
	context->iface_event = event_new(context->event_base, settings->fd, EV_READ, &tunCallback, context);
	if (NULL == context->iface_event) {
		event_base_free(context->event_base);
		free(context);
		return 1;
	};
	if (-1 == event_add(context->iface_event, NULL)) {
		event_free(context->iface_event);
		event_base_free(context->event_base);
		free(context);
		return 1;
	};
	int loop_result;
	loop_result = event_base_loop(context->event_base, 0);
	event_base_free(context->event_base);
	free(context);
	return 0;

};

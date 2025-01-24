#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <event2/event.h>
#include "CaptureContext.h"

#include "emergencyStop.h"
void emergencyStop(struct CaptureContext *context) {
	context->running = false;
	event_base_loopbreak(context->event_base);
	pthread_mutex_lock(&context->rx_mutex);
	pthread_cond_broadcast(&context->rx_cond);
	pthread_mutex_unlock(&context->rx_mutex);
};

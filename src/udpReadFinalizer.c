#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <event2/event.h>
#include "UDPBinding.h"
#include "udpDestroy.h"

#include "udpReadFinalizer.h"
void udpReadFinalizer(struct event *event, void *arg) {
	struct UDPBinding *binding = (struct UDPBinding *) arg;
	pthread_mutex_lock(&binding->mutex);
	binding->read_event = NULL;
	if (!(binding->read_event || binding->write_event)) {
		udpDestroy(binding);
	} else {
		pthread_mutex_unlock(&binding->mutex);
	};
};

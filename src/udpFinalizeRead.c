#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "UDPBinding.h"
#include "udpReadFinalizer.h"

#include "udpFinalizeRead.h"
void udpFinalizeRead(struct UDPBinding *binding) {
	if (binding->read_event && binding->read_alive) {
		event_free_finalize(0, binding->read_event, &udpReadFinalizer);
		binding->read_alive = false;
	};
};

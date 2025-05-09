#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "UDPBinding.h"
#include "udpWriteFinalizer.h"

#include "udpFinalizeWrite.h"
void udpFinalizeWrite(struct UDPBinding *binding) {
	if (binding->write_event && binding->write_alive) {
		event_free_finalize(0, binding->write_event, &udpWriteFinalizer);
		binding->write_alive = false;
	};
};

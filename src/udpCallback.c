#include <event2/event.h>
#include <pthread.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "UDPBinding.h"

#include "udpCallback.h"
void udpCallback(evutil_socket_t fd, short what, void *arg) {
	struct UDPBinding *binding;
	binding = (struct UDPBinding *) arg;
};

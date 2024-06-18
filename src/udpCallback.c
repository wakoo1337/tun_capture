#include <event2/event.h>
#include <pthread.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "CaptureContext.h"

#include "udpCallback.h"
void udpCallback(evutil_socket_t fd, short what, void *arg) {
	struct CaptureContext *context;
	context = (struct CaptureContext *) arg;
};

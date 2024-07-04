#include <sys/time.h>

#include "subTimeval.h"
struct timeval *subTimeval(const struct timeval *a, const struct timeval *b, struct timeval *delta) {
	const long int million = 1000000;
	const long int usec_delta = a->tv_usec - b->tv_usec;
	delta->tv_sec = a->tv_sec - b->tv_sec - ((usec_delta < 0) ? 1 : 0);
	delta->tv_usec = (usec_delta >= 0) ? usec_delta : (usec_delta + million);
	return delta;
};

#include <time.h>
#include <sys/time.h>

#include "getMonotonicTimeval.h"
struct timeval *getMonotonicTimeval(struct timeval *tv) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	tv->tv_sec = ts.tv_sec;
	tv->tv_usec = ts.tv_nsec / 1000;
	return tv;
};

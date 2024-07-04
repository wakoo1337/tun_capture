#include <sys/time.h>

#include "compareTimeval.h"
int compareTimeval(const struct timeval *a, const struct timeval *b) {
	if (a->tv_sec == b->tv_sec) {
		if (a->tv_usec == b->tv_usec) return 0;
		else if (a->tv_usec > b->tv_usec) return 1;
		else return -1;
	} else {
		if (a->tv_sec > b->tv_sec) return 1;
		else return -1;
	};
};

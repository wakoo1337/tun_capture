#include <time.h>

#include "compareTimespec.h"
int compareTimespec(const struct timespec *a, const struct timespec *b) {
	if (a->tv_sec == b->tv_sec) {
		if (a->tv_nsec == b->tv_nsec) return 0;
		else if (a->tv_nsec > b->tv_nsec) return 1;
		else return -1;
	} else {
		if (a->tv_sec > b->tv_sec) return 1;
		else return -1;
	};
};

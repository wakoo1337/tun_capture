#include <sys/time.h>

#include "addTimeval.h"
struct timeval *addTimeval(const struct timeval *a, const struct timeval *b, struct timeval *sum) {
	const long int million = 1000000;
	const long int usec_sum = a->tv_usec + b->tv_usec;
	sum->tv_sec = a->tv_sec + b->tv_sec + ((usec_sum >= million) ? 1 : 0);
	sum->tv_usec = (usec_sum < million) ? usec_sum : (usec_sum - million);
	return sum;
};

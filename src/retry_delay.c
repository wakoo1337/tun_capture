#include <sys/time.h>

#include "retry_delay.h"
const struct timeval retry_delay = {
	.tv_sec = 3,
	.tv_usec = 0
};

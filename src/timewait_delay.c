#include <sys/time.h>

#include "timewait_delay.h"
const struct timeval timewait_delay = {
	.tv_sec = 30,
	.tv_usec = 0
};

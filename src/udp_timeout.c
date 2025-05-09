#include <sys/time.h>

#include "udp_timeout.h"
const struct timeval udp_timeout = {
	.tv_sec = 180,
	.tv_usec = 0
};

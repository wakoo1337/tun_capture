#include <sys/time.h>

#include "segexpire_delay.h"
const struct timeval segexpire_delay = {
	.tv_sec = 10,
	.tv_usec = 0
};

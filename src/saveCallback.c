#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include "set32Bit.h"

#include "saveCallback.h"
void saveCallback(unsigned int count, void *bytes, bool direction, void *user) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	unsigned char header[16];
	set32Bit(&header[0],  (uint32_t) ts.tv_sec);
	set32Bit(&header[4],  (uint32_t) (ts.tv_nsec / 1000));
	set32Bit(&header[8],  (uint32_t) count);
	set32Bit(&header[12], (uint32_t) count);
	fwrite(header, sizeof(unsigned char), 16, (FILE *) user);
	fwrite(bytes,  sizeof(unsigned char), count, (FILE *) user);
	fflush((FILE *) user);
};

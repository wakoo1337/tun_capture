#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include "set32Bit.h"

#include "savePacket.h"
void savePacket(FILE *cap_file, void *bytes, unsigned int count) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	unsigned char header[16];
	set32Bit(&header[0],  (uint32_t) ts.tv_sec);
	set32Bit(&header[4],  (uint32_t) (ts.tv_nsec / 1000));
	set32Bit(&header[8],  (uint32_t) count);
	set32Bit(&header[12], (uint32_t) count);
	fwrite(header, sizeof(unsigned char),    16, cap_file);
	fwrite(bytes,  sizeof(unsigned char), count, cap_file);
	fflush(cap_file);
};

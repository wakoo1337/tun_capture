#include <stdint.h>
#include <string.h>

#include "get32Bit.h"
uint32_t get32Bit(const void *p) {
	uint32_t u;
	memcpy(&u, p, sizeof u);
	return u;
};

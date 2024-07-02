#include <stdint.h>
#include <string.h>

#include "get16Bit.h"
uint16_t get16Bit(const void *p) {
	uint16_t u;
	memcpy(&u, p, sizeof u);
	return u;
};

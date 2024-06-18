#include <stdint.h>
#include <string.h>

#include "get32Bit.h"
uint32_t get32Bit(void *p) {
	uint32_t u;
	memcpy(&u, p, sizeof(uint32_t));
	return u;
};

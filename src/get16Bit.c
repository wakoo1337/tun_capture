#include <stdint.h>
#include <string.h>

#include "get16Bit.h"
uint16_t get16Bit(void *p) {
	uint16_t u;
	memcpy(&u, p, sizeof(uint16_t));
	return u;
};

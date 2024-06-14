#include <string.h>
#include <stdint.h>

#include "set32Bit.h"
void set32Bit(unsigned char *ptr, const uint32_t value) {
	memcpy(ptr, &value, sizeof value);
};

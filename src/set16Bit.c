#include <string.h>
#include <stdint.h>

#include "set16Bit.h"
void set16Bit(unsigned char *ptr, const uint16_t value) {
	memcpy(ptr, &value, sizeof value);
};

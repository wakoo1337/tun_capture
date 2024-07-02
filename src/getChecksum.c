#include <stdbool.h>
#include <stdint.h>
#include "ChecksumContext.h"

#include "getChecksum.h"
uint16_t getChecksum(const struct ChecksumContext *context) {
	return ~(context->acc);
};

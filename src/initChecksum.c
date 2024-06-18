#include <stdbool.h>
#include <stdint.h>
#include "ChecksumContext.h"

#include "initChecksum.h"
void initChecksum(struct ChecksumContext *context) {
	*context = (struct ChecksumContext) {0};
};

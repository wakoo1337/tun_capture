#include <stdint.h>

#include "addShortWithCarry.h"
uint16_t addShortWithCarry(uint16_t a, uint16_t b) {
	const uint16_t sum = a + b;
	return sum + ((sum < a) ? 1 : 0);
}
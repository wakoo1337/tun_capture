#include <stdint.h>
#include <stdbool.h>

#include "checkByteInWindow.h"
bool checkByteInWindow(uint32_t begin, unsigned int length, uint32_t offset) {
	const uint32_t end = begin + length;
	if (end >= begin) {
		return (offset >= begin) && ((offset < end) || (begin == end));
	} else {
		return (offset >= begin) || ((offset < end) || (begin == end));
	};
};

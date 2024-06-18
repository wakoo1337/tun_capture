#include <stdint.h>
#include <stdbool.h>
#include "ChecksumContext.h"
#include "LengthBlob.h"
#include "addShortWithCarry.h"

#include "computeChecksum.h"
void computeChecksum(struct ChecksumContext *context, uint8_t *data, unsigned int length) {
	union {
		uint8_t b[2];
		uint16_t s;
	} conv16;

	if (context->offset) {
		conv16.b[0] = 0;
		conv16.b[1] = data[0];
		context->acc = addShortWithCarry(context->acc, conv16.s);
		data++;
		length--;
	};
	context->offset = (length % 2 != 0);

	const bool aligned = !(((int)data) & 7);
	struct LengthBlob begin_part, mid_part, end_part;

	begin_part.data = data;
	if (aligned) {
		begin_part.size = 0;
		mid_part.data = data;
		mid_part.size = length & -8;
	}
	else {
		begin_part.size = 8 - (((int)data) & 7);
		mid_part.data = (uint8_t*)((((uint64_t)data) + 8) & -8);
		mid_part.size = (length - begin_part.size) & -8;
	};

	end_part.data = mid_part.data + mid_part.size;
	end_part.size = (data + length) - end_part.data;

	uint16_t update = 0;
	bool is_swap = false;

	while (begin_part.size > 0) {
		if (begin_part.size >= 2) {
			conv16.b[0] = begin_part.data[0];
			conv16.b[1] = begin_part.data[1];
			begin_part.data += 2;
			begin_part.size -= 2;
		}
		else {
			conv16.b[0] = begin_part.data[0];
			conv16.b[1] = 0;
			begin_part.data += 1;
			begin_part.size -= 1;
			is_swap = true;
		};
		update = addShortWithCarry(update, conv16.s);
	};

	context->acc = addShortWithCarry(context->acc, update);
	uint64_t acc = 0;

	while (mid_part.size > 0) {
		uint64_t delta;
		delta = *((uint64_t*)mid_part.data);
		acc += delta;
		if (acc < delta) acc++;
		mid_part.data += 8;
		mid_part.size -= 8;
	};
	
	union {
		uint16_t s[4];
		uint64_t ll;
	} conv64;
	
	conv64.ll = acc;
	conv64.ll = ((uint64_t) conv64.s[0]) + ((uint64_t) conv64.s[1]) + ((uint64_t) conv64.s[2]) + ((uint64_t) conv64.s[3]);
	conv64.ll = ((uint64_t) conv64.s[0]) + ((uint64_t) conv64.s[1]) + ((uint64_t) conv64.s[2]) + ((uint64_t) conv64.s[3]);
	update = (uint16_t) conv64.ll;

	while (end_part.size > 0) {
		if (end_part.size >= 2) {
			conv16.b[0] = end_part.data[0];
			conv16.b[1] = end_part.data[1];
			end_part.data += 2;
			end_part.size -= 2;
		}
		else {
			conv16.b[0] = end_part.data[0];
			conv16.b[1] = 0;
			end_part.data += 1;
			end_part.size -= 1;
		};
		update = addShortWithCarry(update, conv16.s);
	};

	if (is_swap) {
		conv16.s = update;
		const uint8_t swap = conv16.b[0];
		conv16.b[0] = conv16.b[1];
		conv16.b[1] = swap;
		update = conv16.s;
	};

	context->acc = addShortWithCarry(context->acc, update);
};

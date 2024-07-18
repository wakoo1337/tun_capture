#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "ChecksumContext.h"
#include "TCPHeaderData.h"
#include "initChecksum.h"
#include "computeChecksum.h"
#include "getChecksum.h"
#include "set16Bit.h"
#include "set32Bit.h"

#include "writeTCPHeader.h"
void writeTCPHeader(uint8_t *data, unsigned int length, struct TCPHeaderData *hdr, uint8_t *pseudo, unsigned int pseudo_len, pthread_mutex_t *unlock_mutex) {
	uint8_t *header = &data[(signed int) -(hdr->data_offset)];
	set16Bit(&header[0], htons(hdr->src_port));
	set16Bit(&header[2], htons(hdr->dst_port));
	set32Bit(&header[4], htonl(hdr->seq_num));
	set32Bit(&header[8], htonl(hdr->ack_num));
	header[12] = (hdr->data_offset << 2) & 240; // hdr->data_offset задаётся отдельной функцией computeTCPDataOffset
	header[13] = (hdr->urg ? 32 : 0) |
		(hdr->ack ? 16 : 0) |
		(hdr->psh ?  8 : 0) |
		(hdr->rst ?  4 : 0) |
		(hdr->syn ?  2 : 0) |
		(hdr->fin ?  1 : 0);
	set16Bit(&header[14], htons(hdr->raw_window));
	set16Bit(&header[16], 0); // Обнуляем контрольную сумму
	set16Bit(&header[18], htons(hdr->urgent_ptr));
	unsigned int options_offset = 20;
	if (hdr->mss_present) {
		header[options_offset] = 2;
		header[options_offset+1] = 4;
		set16Bit(&header[options_offset+2], htons(hdr->mss_value));
		options_offset += 4;
	};
	if (hdr->winscale_present) {
		header[options_offset] = 3;
		header[options_offset+1] = 3;
		header[options_offset+2] = hdr->winscale_value;
		header[options_offset+3] = 1;
		options_offset += 4;
	};
	assert(options_offset == hdr->data_offset);
	struct ChecksumContext checksum;
	initChecksum(&checksum);
	if (unlock_mutex) pthread_mutex_unlock(unlock_mutex);
	computeChecksum(&checksum, pseudo, pseudo_len);
	computeChecksum(&checksum, header, hdr->data_offset + length);
	if (unlock_mutex) pthread_mutex_lock(unlock_mutex);
	set16Bit(&header[16], getChecksum(&checksum));
};

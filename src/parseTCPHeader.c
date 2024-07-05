#include <stdint.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "TCPHeaderData.h"
#include "ChecksumContext.h"
#include "initChecksum.h"
#include "computeChecksum.h"
#include "getChecksum.h"
#include "get16Bit.h"
#include "get32Bit.h"

#include "parseTCPHeader.h"
unsigned int parseTCPHeader(struct TCPHeaderData *hdr, uint8_t *packet, unsigned int count, const uint8_t *pseudo, unsigned int pseudo_length) {
	if (count < 20) return 1;
	*hdr = (struct TCPHeaderData) {0};
	hdr->src_port = ntohs(get16Bit(&packet[0]));
	hdr->src_port = ntohs(get16Bit(&packet[2]));
	hdr->seq_num = ntohl(get32Bit(&packet[4]));
	hdr->ack_num = ntohl(get32Bit(&packet[8]));
	hdr->data_offset = (packet[12] & 240) >> 2;
	hdr->urg = (packet[13] & 32) != 0;
	hdr->ack = (packet[13] & 16) != 0;
	hdr->psh = (packet[13] &  8) != 0;
	hdr->rst = (packet[13] &  4) != 0;
	hdr->syn = (packet[13] &  2) != 0;
	hdr->fin = (packet[13] &  1) != 0;
	hdr->raw_window = ntohs(get16Bit(&packet[14]));
	hdr->checksum = ntohs(get16Bit(&packet[16]));
	hdr->urgent_ptr = ntohs(get16Bit(&packet[18]));
	unsigned int offset = 20;
	while (offset < hdr->data_offset) {
		uint8_t kind = packet[offset];
		switch (kind) {
			case 0: // Конец списка опций
				offset = hdr->data_offset;
				break;
			case 1: // Пустая опция
				offset++;
				break;
			case 2: // Опция MSS
				hdr->mss_present = true;
				if (packet[offset+1] != 4) return 1;
				if (hdr->data_offset <= offset+3) return 1;
				hdr->mss_value = ntohs(get16Bit(&packet[offset+2]));
				offset += 4;
				break;
			case 3: // Опция масштабирования окна
				hdr->winscale_present = true;
				if (packet[offset+1] != 3) return 1;
				if (hdr->data_offset <= offset+2) return 1;
				hdr->winscale_value = packet[offset+2];
				offset += 3;
				break;
			default: // Все остальные опции
				if (hdr->data_offset <= offset+1) return 1;
				offset += packet[offset+1];
				break;
		};
	};
	if (offset > hdr->data_offset) return 1;
	struct ChecksumContext checksum;
	initChecksum(&checksum);
	computeChecksum(&checksum, pseudo, pseudo_length);
	computeChecksum(&checksum, packet, count);
	return (getChecksum(&checksum) == 0) ? 0 : 1;
};

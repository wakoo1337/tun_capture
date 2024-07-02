#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/in.h>
#include "IPv4HeaderData.h"
#include "ChecksumContext.h"
#include "initChecksum.h"
#include "computeChecksum.h"
#include "getChecksum.h"
#include "get16Bit.h"
#include "get32Bit.h"

#include "parseIPv4Header.h"
unsigned int parseIPv4Header(struct IPv4HeaderData *hdr, uint8_t *packet, unsigned int length, uint8_t pseudo[12]) {
	if (length < 20) return 1;
	if ((((uint8_t *) packet)[0] >> 4) != 4) return 1;
	hdr->len = 4 * (((uint8_t *) packet)[0] & 15);
	if (hdr->len < 20) return 1;
	hdr->tos = ((uint8_t *) packet)[1];
	hdr->total_length = ntohs(get16Bit(&((uint8_t *) packet)[2]));
	hdr->identification = ntohs(get16Bit(&((uint8_t *) packet)[4]));
	hdr->df = (((uint8_t *) packet)[6] & 64) != 0;
	hdr->mf = (((uint8_t *) packet)[6] & 32) != 0;
	hdr->fragment_offset = 8 * (ntohs(get16Bit(&((uint8_t *) packet)[6])) & 8191);
	hdr->ttl = ((uint8_t *) packet)[8];
	hdr->protocol = ((uint8_t *) packet)[9];
	hdr->header_cs = ntohs(get16Bit(&((uint8_t *) packet)[10]));
	hdr->src.s_addr = ntohl(get32Bit(&((uint8_t *) packet)[12]));
	hdr->dst.s_addr = ntohl(get32Bit(&((uint8_t *) packet)[16]));
	struct ChecksumContext ctx;
	initChecksum(&ctx);
	computeChecksum(&ctx, packet, hdr->len);
	if (getChecksum(&ctx) != 0) return 1;
	memset(pseudo, 0, 12);
	memcpy(&pseudo[0], &packet[12], 4);
	memcpy(&pseudo[4], &packet[16], 4);
	pseudo[9] = hdr->protocol;
	uint16_t proto_len = htons(hdr->total_length - hdr->len);
	memcpy(&pseudo[10], &proto_len, 2);
	return 0;
};

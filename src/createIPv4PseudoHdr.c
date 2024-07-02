#include <netinet/in.h>
#include <string.h>
#include <stdint.h>

#include "createIPv4PseudoHdr.h"
void createIPv4PseudoHdr(void *pseudo, const struct sockaddr *src, const struct sockaddr *dst, uint8_t protocol, unsigned int length) {
	memset(pseudo, 0, 12);
	memcpy(&((uint8_t *) pseudo)[0], &((const struct sockaddr_in *) src)->sin_addr.s_addr, sizeof(in_addr_t));
	memcpy(&((uint8_t *) pseudo)[4], &((const struct sockaddr_in *) dst)->sin_addr.s_addr, sizeof(in_addr_t));
	((uint8_t *) pseudo)[9] = protocol;
	((uint16_t *) pseudo)[5] = htons((uint16_t) length);
};

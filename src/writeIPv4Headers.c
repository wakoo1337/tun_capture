#include <stdint.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "ForwardingMappingIPv4.h"
#include "ForwardingMappingIPv6.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "IPFragmentMetadata.h"
#include "ChecksumContext.h"
#include "initChecksum.h"
#include "computeChecksum.h"
#include "getChecksum.h"

#include "writeIPv4Headers.h"
void writeIPv4Headers(struct CaptureContext *context, struct IPFragmentMetadata *metadatas, unsigned int count, uint8_t protocol, const struct sockaddr *src, const struct sockaddr *dst) {
	for (unsigned int i=0;i < count;i++) {
		struct IPFragmentMetadata *metadata = &metadatas[i];
		metadata->buffer[0] = 69; // IPv4 и заголовок в 20 байтов, грязный извращенец!
		metadata->buffer[1] = 0;
		((uint16_t *) metadata->buffer)[1] = htons(metadata->header_size + metadata->data_size);
		((uint16_t *) metadata->buffer)[2] = htons(context->ipv4_id);
		((uint16_t *) metadata->buffer)[3] = htons(((metadata->fragment_offset / 8) & 8191) | ((i != (count-1)) ? 8192 : 0));
		metadata->buffer[8] = context->settings->ttl;
		metadata->buffer[9] = protocol;
		((uint16_t *) metadata->buffer)[5] = 0;
		memcpy(&metadata->buffer[12], &((struct sockaddr_in *) src)->sin_addr, sizeof(struct in_addr));
		memcpy(&metadata->buffer[16], &((struct sockaddr_in *) dst)->sin_addr, sizeof(struct in_addr));
		struct ChecksumContext context;
		initChecksum(&context);
		computeChecksum(&context, &metadata->buffer[0], 20);
		((uint16_t *) metadata->buffer)[5] = getChecksum(&context);
	};
	context->ipv4_id++;
};

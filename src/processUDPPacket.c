#include <stdint.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "UDPNetworkProtocolStrategy.h"
#include "UDPHeaderData.h"
#include "ChecksumContext.h"
#include "get16Bit.h"
#include "set16Bit.h"
#include "initChecksum.h"
#include "computeChecksum.h"
#include "getChecksum.h"

#include "processUDPPacket.h"
unsigned int processUDPPacket(void *free_me, void *packet, unsigned int packet_len, void *pseudo, const struct UDPNetworkProtocolStrategy *strategy, struct sockaddr *sa) {
	struct UDPHeaderData hdr;
	if (packet_len < 8) return 1;
	hdr.src_port = ntohs(get16Bit(&((uint8_t *) packet)[0]));
	hdr.dst_port = ntohs(get16Bit(&((uint8_t *) packet)[2]));
	hdr.length = ntohs(get16Bit(&((uint8_t *) packet)[4]));
	if (hdr.length != packet_len) return 1;
	hdr.checksum = ntohs(get16Bit(&((uint8_t *) packet)[6]));
	struct ChecksumContext ctx;
	uint16_t computed_cs;
	if ((hdr.checksum == 0) || (
		set16Bit(&((uint8_t *) packet)[6], 0),
		initChecksum(&ctx),
		computeChecksum(&ctx, pseudo, strategy->pseudo_length),
		computeChecksum(&ctx, packet, packet_len),
		(computed_cs = getChecksum(&ctx)),
		((computed_cs == hdr.checksum) || ((hdr.checksum == 0xFFFF) && (computed_cs == 0)))
		)) {
		// Проверка контрольной суммы прошла успешно. Нужно вставить в очередь на отправку на соответствующий адрес и порт
		return 0;
	};
	return 1;
};

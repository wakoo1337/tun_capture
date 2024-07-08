#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"

#include "tcpConnwaitPacketsProcessor.h"
unsigned int tcpConnwaitPacketsProcessor(const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	free(payload->free_me);
	return 0;
};

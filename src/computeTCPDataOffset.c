#include <stdint.h>
#include <stdbool.h>
#include "TCPHeaderData.h"

#include "computeTCPDataOffset.h"
unsigned int computeTCPDataOffset(struct TCPHeaderData *hdr) {
	return 20 + (hdr->mss_present ? 4 : 0) + (hdr->winscale_present ? 4 : 0);
};

#include <sys/socket.h>
#include <stdint.h>
#include <pthread.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "setIPv4SockaddrPort.h"
#include "getIPv4SockaddrPort.h"
#include "computeIPv4FragmentsCount.h"
#include "fillIPv4FragmentMetadatas.h"
#include "createIPv4PseudoHdr.h"
#include "writeIPv4Headers.h"
#include "computeIPv4MSS.h"

#include "ipv4_strategy.h"
const struct NetworkProtocolStrategy ipv4_strategy = {
	.pseudo_length = 12,
	.port_setter = &setIPv4SockaddrPort,
	.port_getter = &getIPv4SockaddrPort,
	.compute_fragcount = &computeIPv4FragmentsCount,
	.fill_metadatas = &fillIPv4FragmentMetadatas,
	.create_pseudo = &createIPv4PseudoHdr,
	.write_headers = &writeIPv4Headers,
	.compute_mss = &computeIPv4MSS
};

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/socket.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "ipv4_strategy.h"
#include "ipv6_strategy.h"

#include "addressFamilyToNetworkStrategy.h"
const struct NetworkProtocolStrategy *addressFamilyToNetworkStrategy(int af) {
	switch (af) {
		case AF_INET:
			return &ipv4_strategy;
		break;
		case AF_INET6:
			return &ipv6_strategy;
		break;
		default:
			return NULL;
	};
};

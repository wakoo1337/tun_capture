#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPSiteQueueItem.h"
#include "TCPConnection.h"
#include "MAX_SITE_QUEUE.h"

#include "getSendWindowSize.h"
uint16_t getSendWindowSize(struct TCPConnection *conn) {
	if (conn->site_scheduled > MAX_SITE_QUEUE) return 0;
	else {
		const unsigned int scaled = (MAX_SITE_QUEUE - conn->site_scheduled) >> (conn->scaling_enabled ? conn->our_scale : 0);
		return (scaled > 65535) ? 65535 : scaled;
	};
};

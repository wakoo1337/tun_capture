#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "IPPacketPayload.h"
#include "TCPSitePrequeueItem.h"
#include "TCPHeaderData.h"
#include "fillPrequeueItem.h"
#include "prequeueItemToSiteData.h"

#include "processTCPPacketImmediately.h"
unsigned int processTCPPacketImmediately(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header, unsigned int (*on_fin)(struct TCPConnection *)) {
	if (payload->count > header->data_offset || header->fin) {
		struct TCPSitePrequeueItem pq_item;
		fillPrequeueItem(&pq_item, connection, payload, header);
		if (prequeueItemToSiteData(connection, &pq_item)) return 1;
		if (pq_item.fin && on_fin(connection)) return 1;
	};
	return 0;
};

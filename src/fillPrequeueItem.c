#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPSitePrequeueItem.h"
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"

#include "fillPrequeueItem.h"
void fillPrequeueItem(struct TCPSitePrequeueItem *item, struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	item->seq = header->seq_num;
	item->urgent_count = header->urg ? header->urgent_ptr : 0;
	item->data_count = payload->count - header->data_offset - item->urgent_count;
	item->data = ((item->data_count != 0) || (item->urgent_count != 0)) ? (payload->packet + header->data_offset) : NULL;
	item->connection = connection;
	item->timeout = NULL;
	item->free_me = ((item->data_count != 0) || (item->urgent_count != 0)) ? payload->free_me : NULL;
	item->fin = header->fin;
	item->deleteable = true;
};

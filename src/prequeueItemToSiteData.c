#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPSitePrequeueItem.h"
#include "TCPSiteQueueItem.h"
#include "TCPConnection.h"

#include "prequeueItemToSiteData.h"
unsigned int prequeueItemToSiteData(struct TCPConnection *connection, struct TCPSitePrequeueItem *pq_item) {
	struct TCPSiteQueueItem *q_item;
	q_item = malloc(sizeof(struct TCPSiteQueueItem));
	if (NULL == q_item) return 1;
	q_item->buffer = pq_item->data;
	q_item->urgent_count = pq_item->urgent_count;
	q_item->data_count = pq_item->data_count;
	q_item->already_sent = 0;
	q_item->free_me = pq_item->free_me;
	q_item->next = NULL;
	*connection->site_last = q_item;
	connection->site_last = &q_item->next;
	const unsigned int total = q_item->urgent_count + q_item->data_count;
	connection->site_scheduled += total;
	connection->first_desired += total;
	return 0;
};

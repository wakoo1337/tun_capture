#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "contrib/avl.h"
#include "contrib/heap.h"
#include "IPPacketPayload.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPHeaderData.h"
#include "TCPSitePrequeueItem.h"
#include "CaptureContext.h"
#include "getMonotonicTimeval.h"
#include "addTimeval.h"
#include "checkByteInWindow.h"
#include "enqueueTimeout.h"
#include "tcpDeleteExpiredSegment.h"
#include "enqueueSiteDataFromPrequeueItem.h"
#include "tcpUpdateEvent.h"
#include "cancelTimeout.h"
#include "startTimer.h"
#include "sendTCPAcknowledgement.h"
#include "tcpCleanupConfirmed.h"
#include "segexpire_delay.h"
#include "MAX_SITE_QUEUE.h"

#include "tcpEstablishedPacketsProcessor.h"
unsigned int tcpEstablishedPacketsProcessor(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	const uint32_t last = header->seq_num + (payload->count - header->data_offset) - ((payload->count - header->data_offset) ? 1 : 0);
	const uint32_t old_first = connection->first_desired;
	if (checkByteInWindow(connection->first_desired, MAX_SITE_QUEUE - connection->site_scheduled, header->seq_num) && checkByteInWindow(connection->first_desired, MAX_SITE_QUEUE - connection->site_scheduled, last)) {
		struct TCPSitePrequeueItem *item;
		item = malloc(sizeof(struct TCPSitePrequeueItem));
		if (NULL == item) return 1;
		item->seq = header->seq_num;
		void **probe;
		probe = avl_probe(connection->site_prequeue, item);
		if ((probe == NULL) || ((*probe) != item)) {
			free(item);
			return 1;
		};
		item->data = payload->packet + header->data_offset;
		item->urgent_count = header->urg ? header->urgent_ptr : 0;
		item->packet_ack = header->ack_num;
		item->window = header->raw_window << (connection->scaling_enabled ? connection->remote_scale : 0);
		item->data_count = payload->count - header->data_offset - item->urgent_count;
		item->connection = connection;
		struct timeval now, timeout;
		getMonotonicTimeval(&now);
		addTimeval(&segexpire_delay, &now, &timeout);
		item->timeout = enqueueTimeout(connection->context, &timeout, &tcpDeleteExpiredSegment, item);
		item->free_me = payload->free_me;
		pthread_mutex_lock(&connection->context->timeout_mutex);
		startTimer(connection->context);
		pthread_mutex_unlock(&connection->context->timeout_mutex);
	} else free(payload->free_me);
	struct TCPSitePrequeueItem *found_prequeue;
	while ((found_prequeue = avl_find(connection->site_prequeue, &connection->first_desired)), found_prequeue) {
		cancelTimeout(connection->context, &found_prequeue->timeout);
		connection->first_desired = found_prequeue->seq + found_prequeue->urgent_count + found_prequeue->data_count;
		connection->latest_ack = found_prequeue->packet_ack;
		connection->app_window = found_prequeue->window;
		enqueueSiteDataFromPrequeueItem(connection, found_prequeue);
		tcpUpdateEvent(connection);
	};
	tcpCleanupConfirmed(connection);
	if (old_first != connection->first_desired) sendTCPAcknowledgement(connection);
	return 0;
};

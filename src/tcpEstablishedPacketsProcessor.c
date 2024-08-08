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
#include "tcpstate_gotfin.h"
#include "MAX_SITE_QUEUE.h"

#include "tcpEstablishedPacketsProcessor.h"
unsigned int tcpEstablishedPacketsProcessor(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	const uint32_t last = header->seq_num + (payload->count - header->data_offset);
	const uint32_t old_first = connection->first_desired;
	if (!(checkByteInWindow(connection->first_desired, MAX_SITE_QUEUE - connection->site_scheduled, header->seq_num) &&
	checkByteInWindow(connection->first_desired, MAX_SITE_QUEUE - connection->site_scheduled, last))) {
		// Если не все байты сегмента лежат в окне приёма, он игнорируется
		free(payload->free_me);
		return 0;
	};
	if (payload->count > header->data_offset) {
		// Если в пакете есть данные
		struct TCPSitePrequeueItem *item;
		item = malloc(sizeof(struct TCPSitePrequeueItem));
		if (NULL == item) {
			free(payload->free_me);
			return 1;
		};
		item->seq = header->seq_num;
		void **probe;
		probe = avl_probe(connection->site_prequeue, item);
		if (probe == NULL) {
			free(item);
			free(payload->free_me);
			return 1;
		} else if ((*probe) != item) {
			free(item);
			free(payload->free_me);
			return 0;
		};
		item->data = payload->packet + header->data_offset;
		item->urgent_count = header->urg ? header->urgent_ptr : 0;
		item->data_count = payload->count - header->data_offset - item->urgent_count;
		item->connection = connection;
		item->free_me = payload->free_me;
		pthread_mutex_unlock(&connection->mutex);
		pthread_mutex_lock(&connection->context->timeout_mutex);
		struct timeval now, timeout;
		getMonotonicTimeval(&now);
		addTimeval(&segexpire_delay, &now, &timeout);
		item->timeout = enqueueTimeout(connection->context, &timeout, &tcpDeleteExpiredSegment, item, &connection->mutex);
		startTimer(connection->context);
		pthread_mutex_lock(&connection->mutex);
		pthread_mutex_unlock(&connection->context->timeout_mutex);
	};
	if (checkByteInWindow(connection->latest_ack, connection->app_scheduled + 1, header->ack_num)) { // Обновляем последний ACK и размер окна
		connection->latest_ack = header->ack_num;
		connection->app_window = header->raw_window << (connection->scaling_enabled ? connection->remote_scale : 0);
	};
	struct TCPSitePrequeueItem *found_prequeue;
	while ((found_prequeue = avl_find(connection->site_prequeue, &connection->first_desired))) {
		pthread_mutex_unlock(&connection->mutex);
		cancelTimeout(connection->context, &found_prequeue->timeout);
		pthread_mutex_lock(&connection->mutex);
		connection->first_desired += found_prequeue->urgent_count + found_prequeue->data_count;
		enqueueSiteDataFromPrequeueItem(connection, found_prequeue);
	};
	tcpCleanupConfirmed(connection);
	tcpUpdateEvent(connection);
	if (header->fin && (header->seq_num == connection->first_desired)) {
		connection->first_desired++;
		connection->state = &tcpstate_gotfin;
	};
	if (old_first != connection->first_desired) sendTCPAcknowledgement(connection);
	return 0;
};

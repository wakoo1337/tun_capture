#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "contrib/avl.h"
#include "contrib/logdel_heap.h"
#include "IPPacketPayload.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPHeaderData.h"
#include "TCPSitePrequeueItem.h"
#include "CaptureContext.h"
#include "TCPAppQueueItem.h"
#include "getMonotonicTimeval.h"
#include "addTimeval.h"
#include "checkByteInWindow.h"
#include "enqueueTimeout.h"
#include "tcpDeleteExpiredSegment.h"
#include "prequeueItemToSiteData.h"
#include "tcpUpdateReadEvent.h"
#include "tcpUpdateWriteEvent.h"
#include "cancelTimeout.h"
#include "startTimer.h"
#include "sendTCPAcknowledgement.h"
#include "tcpCleanupConfirmed.h"
#include "isNewAckAcceptable.h"
#include "scaleRemoteWindow.h"
#include "enqueueTCPPacketTransmission.h"
#include "enqueueUnsentTCPPacketsTransmission.h"
#include "cancelTimeoutUnlocked.h"
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
	if (payload->count > header->data_offset || header->fin) {
		// Если в пакете есть данные или установлен флаг FIN
		struct TCPSitePrequeueItem *item;
		item = malloc(sizeof(struct TCPSitePrequeueItem));
		if (NULL == item) {
			free(payload->free_me);
			return 1;
		};
		item->seq = header->seq_num;
		item->data = payload->packet + header->data_offset;
		item->urgent_count = header->urg ? header->urgent_ptr : 0;
		item->data_count = payload->count - header->data_offset - item->urgent_count;
		item->connection = connection;
		item->timeout = NULL;
		item->free_me = payload->free_me;
		item->fin = header->fin;
		pthread_mutex_unlock(&connection->mutex);
		pthread_mutex_lock(&connection->context->timeout_mutex);
		pthread_mutex_lock(&connection->mutex);
		struct timeval now, timeout;
		getMonotonicTimeval(&now);
		addTimeval(&segexpire_delay, &now, &timeout);
		item->timeout = enqueueTimeout(connection->context, &timeout, &tcpDeleteExpiredSegment, item, &connection->mutex);
		if (NULL == item->timeout) {
			free(item);
			free(payload->free_me);
			return 1;
		};
		startTimer(connection->context);
		pthread_mutex_unlock(&connection->context->timeout_mutex);
		void **probe;
		probe = avl_probe(connection->site_prequeue, item);
		if (NULL == probe) {
			cancelTimeout(connection->context, &connection->mutex, &item->timeout);
			free(item);
			free(payload->free_me);
			return 1;
		} else if ((*probe) != item) {
			cancelTimeout(connection->context, &connection->mutex, &item->timeout);
			free(item);
			free(payload->free_me);
			return 0;
		};
	} else free(payload->free_me);
	if (isNewAckAcceptable(connection, header->ack_num)) { // Обновляем последний ACK и размер окна
		connection->latest_ack = header->ack_num;
		connection->app_window = scaleRemoteWindow(connection, header->raw_window);
	};
	tcpCleanupConfirmed(connection);
	enqueueUnsentTCPPacketsTransmission(connection);
	struct TCPSitePrequeueItem *found_prequeue;
	while ((found_prequeue = avl_find(connection->site_prequeue, &connection->first_desired))) {
		pthread_mutex_unlock(&connection->mutex);
		pthread_mutex_lock(&connection->context->timeout_mutex);
		pthread_mutex_lock(&connection->mutex);
		if ((found_prequeue = avl_find(connection->site_prequeue, &connection->first_desired))) {
			// За время ожидания на мьютексах таймер может сработать, а элемент предочереди удалиться
			cancelTimeoutUnlocked(connection->context, &found_prequeue->timeout);
			pthread_mutex_unlock(&connection->context->timeout_mutex);
			prequeueItemToSiteData(connection, found_prequeue);
			if (found_prequeue->fin) {
				connection->first_desired++;
				connection->state = &tcpstate_gotfin;
				event_add(connection->write_event, NULL);
				free(found_prequeue);
				return sendTCPAcknowledgement(connection);
			};
			free(found_prequeue);
		} else pthread_mutex_unlock(&connection->context->timeout_mutex);
	};
	tcpUpdateReadEvent(connection);
	tcpUpdateWriteEvent(connection);
	if (old_first != connection->first_desired) sendTCPAcknowledgement(connection);
	return 0;
};

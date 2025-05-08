#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "contrib/avl.h"
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "IPPacketPayload.h"
#include "TCPHeaderData.h"
#include "TCPSitePrequeueItem.h"
#include "getMonotonicTimeval.h"
#include "addTimeval.h"
#include "enqueueTimeout.h"
#include "cancelTimeout.h"
#include "startTimer.h"
#include "tcpDeleteExpiredSegment.h"
#include "fillPrequeueItem.h"
#include "segexpire_delay.h"

#include "addPacketToPrequeue.h"
unsigned int addPacketToPrequeue(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header) {
	if (payload->count > header->data_offset || header->fin) {
		// Если в пакете есть данные или установлен флаг FIN
		struct TCPSitePrequeueItem *item;
		item = malloc(sizeof(struct TCPSitePrequeueItem));
		if (NULL == item) {
			free(payload->free_me);
			return 1;
		};
		fillPrequeueItem(item, connection, payload, header);
		pthread_mutex_unlock(&connection->mutex);
		pthread_mutex_lock(&connection->context->timeout_mutex);
		pthread_mutex_lock(&connection->mutex);
		void **probe;
		probe = avl_probe(connection->site_prequeue, item);
		if (*probe != item) {
			free(item);
			free(payload->free_me);
			pthread_mutex_unlock(&connection->context->timeout_mutex);
			return (NULL == probe); // Возвращает true, если не удалось выделить память
		};
		struct timeval now, timeout;
		getMonotonicTimeval(&now);
		addTimeval(&segexpire_delay, &now, &timeout);
		item->timeout = enqueueTimeout(connection->context, &timeout, &tcpDeleteExpiredSegment, item);
		if (NULL == item->timeout) {
			free(item);
			free(payload->free_me);
			pthread_mutex_unlock(&connection->context->timeout_mutex);
			return 1;
		};
		startTimer(connection->context);
		pthread_mutex_unlock(&connection->context->timeout_mutex);
	};
	return 0;
};

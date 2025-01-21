#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include "contrib/avl.h"
#include "contrib/logdel_heap.h"
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"
#include "TCPSiteQueueItem.h"
#include "PacketQueueItem.h"
#include "tcpDestroySitePrequeueItem.h"
#include "cancelTimeout.h"
#include "freeNoRefsAppQueueItem.h"
#include "incrementAppQueueItemRefCount.h"
#include "decrementAppQueueItemRefCount.h"
#include "sendTCPPacketRefcounted.h"
#include "processTCPData.h"

#include "destroyTCPConnection.h"
void destroyTCPConnection(struct TCPConnection *connection) {
	// ФУНКЦИЯ ДОЛЖНА ВЫПОЛНЯТЬСЯ СТРОГО В ГЛАВНОМ ПОТОКЕ И ИЗ ФИНАЛИЗАТОРА
	pthread_mutex_unlock(&connection->mutex); // Избегаем взаимной блокировки
	pthread_mutex_lock(&connection->context->tcp_mutex);
	pthread_mutex_lock(&connection->mutex);
	void *del;
	del = avl_delete(connection->context->tcp_connections, connection);
	assert((del == NULL) || (del == connection));
	pthread_mutex_unlock(&connection->context->tcp_mutex);
	struct TCPAppQueueItem *app_queue;
	app_queue = connection->app_queue;
	connection->app_queue = NULL;
	connection->app_last = &connection->app_queue;
	while (app_queue) {
		incrementAppQueueItemRefCount(app_queue);
		struct TCPAppQueueItem *next;
		next = app_queue->next;
		cancelTimeout(connection->context, &connection->mutex, &app_queue->timeout);
		decrementAppQueueItemRefCount(app_queue);
		decrementAppQueueItemRefCount(app_queue);
		freeNoRefsAppQueueItem(app_queue);
		app_queue = next;
	};
	avl_destroy(connection->site_prequeue, &tcpDestroySitePrequeueItem);
	while (connection->site_queue) {
		struct TCPSiteQueueItem *next;
		next = connection->site_queue->next;
		free(connection->site_queue->free_me);
		free(connection->site_queue);
		connection->site_queue = next;
	};
	pthread_mutex_unlock(&connection->mutex);
	pthread_mutex_lock(&connection->context->rx_mutex);
	pthread_mutex_lock(&connection->mutex);
	struct PacketQueueItem *rx_current, **rx_prevnext;
	rx_current = connection->context->rx_begin;
	rx_prevnext = &connection->context->rx_begin;
	while (rx_current) {
		if ((rx_current->processor == &processTCPData) && (((struct TCPConnection *) rx_current->arg) == connection)) {
			struct PacketQueueItem *next;
			next = rx_current->next;
			free(rx_current->free_me);
			free(rx_current);
			*rx_prevnext = next;
			rx_current = next;
		} else {
			rx_prevnext = &rx_current->next;
			rx_current = *rx_prevnext;
		};
	};
	if (NULL == connection->context->rx_begin) connection->context->rx_end = &connection->context->rx_begin;
	pthread_mutex_unlock(&connection->context->rx_mutex);
	pthread_mutex_lock(&connection->context->tx_mutex);
	struct PacketQueueItem *tx_current, **tx_prevnext;
	tx_current = connection->context->tx_begin;
	tx_prevnext = &connection->context->tx_begin;
	while (tx_current) {
		if ((tx_current->processor == &sendTCPPacketRefcounted) && (((struct TCPAppQueueItem *) tx_current->arg)->connection == connection)) {
			struct TCPAppQueueItem *item = (struct TCPAppQueueItem *) tx_current->arg;
			struct PacketQueueItem *next;
			next = tx_current->next;
			decrementAppQueueItemRefCount(item);
			freeNoRefsAppQueueItem(item);
			free(tx_current);
			*tx_prevnext = next;
			tx_current = next;
		} else {
			tx_prevnext = &tx_current->next;
			tx_current = *tx_prevnext;
		};
	};
	if (NULL == connection->context->tx_begin) connection->context->tx_end = &connection->context->tx_begin;
	pthread_mutex_unlock(&connection->context->tx_mutex);
	pthread_mutex_unlock(&connection->mutex);
	pthread_mutex_destroy(&connection->mutex);
	close(connection->sock);
	free(connection);
}; 

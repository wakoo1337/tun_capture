#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <semaphore.h>
#include "contrib/avl.h"
#include "SrcDstSockaddrs.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "TCPConnection.h"
#include "TCPAppQueueItem.h"
#include "TCPSiteQueueItem.h"
#include "PacketQueueItem.h"
#include "cancelTimeout.h"
#include "freeNoRefsAppQueueItem.h"
#include "decrementAppQueueItemRefCount.h"
#include "sendTCPPacketRefcounted.h"
#include "processTCPData.h"
#include "tcpDestroySitePrequeue.h"
#include "tcpDestroyAppPrequeue.h"

#include "destroyTCPConnection.h"
void destroyTCPConnection(struct TCPConnection *connection) {
	pthread_mutex_unlock(&connection->mutex);
	pthread_mutex_lock(&connection->context->tcp_mutex);
	pthread_mutex_lock(&connection->mutex);
	void *del;
	del = avl_delete(connection->context->tcp_connections, connection);
	assert(del == connection);
	pthread_mutex_unlock(&connection->context->tcp_mutex);
	pthread_mutex_unlock(&connection->mutex); // Удалили соединение из дерева
	pthread_mutex_lock(&connection->context->rx_mutex);
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
	pthread_mutex_unlock(&connection->context->rx_mutex); // Удалили все соответствующие соединению принятые пакеты
	for (unsigned int i=0;i < connection->context->settings->threads_count;i++) sem_wait(&connection->semaphore); // Блокируем все места в семафоре, чтобы убедиться, что в рабочих потоках ничего не выполняется
	pthread_mutex_lock(&connection->mutex);
	cancelTimeout(connection->context, &connection->mutex, &connection->timewait_item);
	while (connection->app_queue) {
		struct TCPAppQueueItem *next;
		next = connection->app_queue->next;
		cancelTimeout(connection->context, &connection->mutex, &connection->app_queue->timeout);
		decrementAppQueueItemRefCount(connection->app_queue);
		freeNoRefsAppQueueItem(connection->app_queue);
		connection->app_queue = next;
	};
	while (connection->site_queue) {
		struct TCPSiteQueueItem *next;
		next = connection->site_queue->next;
		free(connection->site_queue->free_me);
		free(connection->site_queue);
		connection->site_queue = next;
	};
	tcpDestroySitePrequeue(connection);
	tcpDestroyAppPrequeue(connection);
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
	for (unsigned int i=0;i < connection->context->settings->threads_count;i++) sem_post(&connection->semaphore);
	sem_destroy(&connection->semaphore);
	close(connection->sock);
	free(connection);
};

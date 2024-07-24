#include <pthread.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "contrib/heap.h"
#include "CaptureContext.h"
#include "SrcDstSockaddrs.h"
#include "TCPSiteQueueItem.h"
#include "TCPConnection.h"
#include "PacketQueueItem.h"
#include "dequeueSiteQueueItem.h"
#include "tcpUpdateEvent.h"
#include "processTCPUrgentData.h"
#include "processTCPData.h"
#include "enqueuePacket.h"
#include "HEADERS_RESERVE.h"
#include "MAX_APP_QUEUE.h"

#include "tcpEstablishedEventCallback.h"
void tcpEstablishedEventCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	if (what & EV_READ) {
		while (connection->app_scheduled < MAX_APP_QUEUE) {
			pthread_mutex_unlock(&connection->mutex);
			struct PacketQueueItem *item;
			item = malloc(sizeof(struct PacketQueueItem));
			if (NULL == item) break;
			uint8_t *buffer;
			buffer = malloc(HEADERS_RESERVE + connection->max_pktdata);
			if (NULL == buffer) {
				free(item);
				pthread_mutex_lock(&connection->mutex);
				break;
			};
			ssize_t received;
			data_rx_retry:
			received = recv(connection->sock, &buffer[HEADERS_RESERVE], connection->max_pktdata, 0);
			if (received < 0) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					free(buffer);
					free(item);
					pthread_mutex_lock(&connection->mutex);
					break;
				} else if (errno == EINTR) goto data_rx_retry;
				else {
					free(buffer);
					free(item);
					pthread_mutex_lock(&connection->mutex);
					break; // TODO обработать ошибку
				};
			} else if (received > 0) {
				buffer = realloc(buffer, HEADERS_RESERVE + received);
				item->data = &buffer[HEADERS_RESERVE];
				item->count = received;
				item->processor = &processTCPData;
				item->mutex = &connection->mutex;
				item->free_me = buffer;
				item->arg = connection;
				pthread_mutex_lock(&connection->context->queue_mutex);
				enqueuePacket(connection->context, item);
				pthread_cond_signal(&connection->context->queue_cond);
				pthread_mutex_unlock(&connection->context->queue_mutex);
				pthread_mutex_lock(&connection->mutex); // Разблокировать не надо, он будет нужен либо на следующей итерации цикла, либо на следующем этапе
				connection->app_scheduled += received;
			} else {
				free(buffer);
				free(item);
				pthread_mutex_lock(&connection->mutex);
				break; // TODO сменить состояние
			};
		};
	};
	if (what & EV_WRITE) {
		struct TCPSiteQueueItem *item;
		while (true) {
			ssize_t written;
			item = dequeueSiteQueueItem(connection);
			if (NULL == item) break;
			if (item->already_sent < item->urgent_count) {
				urgent_tx_retry:
				written = send(connection->sock, item->buffer + item->already_sent, item->urgent_count - item->already_sent, MSG_OOB);
				if (-1 == written) {
					if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) break;
					else if (errno == EINTR) goto urgent_tx_retry;
					else {
						break;
						// TODO обработать ошибку
					};
				} else {
					item->already_sent += written;
					connection->site_scheduled -= written;
				};
			};
			if (item->already_sent >= item->urgent_count) {
				data_tx_retry:
				written = send(connection->sock, item->buffer + item->already_sent, item->urgent_count + item->data_count - item->already_sent, 0);
				if (-1 == written) {
					if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) break;
					else if (errno == EINTR) goto data_tx_retry;
					else {
						break;
						// TODO обработать ошибку
					};
				} else {
					item->already_sent += written;
					connection->site_scheduled -= written;
				};
			};
			if (item->already_sent == item->urgent_count + item->data_count) {
				free(item->free_me);
				free(item);
			};
		};
	};
	tcpUpdateEvent(connection);
	pthread_mutex_unlock(&connection->mutex);
};

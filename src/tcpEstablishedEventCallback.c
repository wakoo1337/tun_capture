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
#include "HEADERS_RESERVE.h"

#include "tcpEstablishedEventCallback.h"
void tcpEstablishedEventCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	pthread_mutex_unlock(&connection->mutex);
	if (what & EV_READ) {
		struct PacketQueueItem *queue, **last;
		last = &queue;
		while (true) {
			(*last) = malloc(sizeof(struct PacketQueueItem));
			if (NULL == (*last)) break;
			uint8_t *buffer;
			buffer = malloc(HEADERS_RESERVE + connection->max_pktdata);
			if (NULL == buffer) {
				free(*last);
				(*last) = NULL;
				break;
			};
			urgent_rx_retry:
			ssize_t readed;
			readed = recv(connection->sock, &buffer[HEADERS_RESERVE], connection->max_pktdata, MSG_OOB);
			if (readed == -1) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					free(*last);
					(*last) = NULL;
					free(buffer);
					break;
				} else if (errno == EINTR) goto urgent_rx_retry;
				else {
					// TODO обработать ошибку
				};
			};
			if (readed == 0) {
				free(*last);
				(*last) = NULL;
				free(buffer);
				break;
			};
			buffer = realloc(buffer, HEADERS_RESERVE + readed);
			(*last)->data = &buffer[HEADERS_RESERVE];
			(*last)->count = readed;
			(*last)->processor = NULL;
			(*last)->free_me = buffer;
			(*last)->arg = connection;
			(*last)->next = NULL;
			last = &((*last)->next);
		};
		while (true) {
			(*last) = malloc(sizeof(struct PacketQueueItem));
			if (NULL == (*last)) break;
			uint8_t *buffer;
			buffer = malloc(HEADERS_RESERVE + connection->max_pktdata);
			if (NULL == buffer) {
				free(*last);
				(*last) = NULL;
				break;
			};
			data_rx_retry:
			ssize_t readed;
			readed = recv(connection->sock, &buffer[HEADERS_RESERVE], connection->max_pktdata, 0);
			if (readed == -1) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					free(*last);
					(*last) = NULL;
					free(buffer);
					break;
				} else if (errno == EINTR) goto data_rx_retry;
				else {
					// TODO обработать ошибку
				};
			};
			if (readed == 0) {
				free(*last);
				(*last) = NULL;
				free(buffer);
				break;
			};
			buffer = realloc(buffer, HEADERS_RESERVE + readed);
			(*last)->data = &buffer[HEADERS_RESERVE];
			(*last)->count = readed;
			(*last)->processor = NULL;
			(*last)->free_me = buffer;
			(*last)->arg = connection;
			(*last)->next = NULL;
			last = &((*last)->next);
		};
		*(connection->context->captured_end) = queue;
	};
	pthread_mutex_lock(&connection->mutex);
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

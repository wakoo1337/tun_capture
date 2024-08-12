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
#include "tcpUpdateWriteEvent.h"
#include "processTCPUrgentData.h"
#include "processTCPData.h"
#include "enqueueRxPacket.h"
#include "tcpUpdateReadEvent.h"
#include "HEADERS_RESERVE.h"
#include "MAX_APP_QUEUE.h"

#include "tcpEstablishedReadCallback.h"
unsigned int tcpEstablishedReadCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	if (what & EV_READ) {
		while (connection->app_scheduled < MAX_APP_QUEUE) {
			pthread_mutex_unlock(&connection->mutex);
			struct PacketQueueItem *item;
			item = malloc(sizeof(struct PacketQueueItem));
			if (NULL == item) return 1;
			uint8_t *buffer;
			buffer = malloc(HEADERS_RESERVE + connection->max_pktdata);
			if (NULL == buffer) {
				free(item);
				return 1;
			};
			ssize_t received;
			data_rx_retry:
			received = recv(connection->sock, &buffer[HEADERS_RESERVE], connection->max_pktdata, 0);
			if (received < 0) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					free(buffer);
					free(item);
					return 0;
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
				enqueueRxPacket(connection->context, item);
				pthread_mutex_lock(&connection->mutex); // Разблокировать не надо, он будет нужен либо на следующей итерации цикла, либо при выходе
				connection->app_scheduled += received;
			} else {
				free(buffer);
				free(item);
				pthread_mutex_lock(&connection->mutex);
				break; // TODO сменить состояние
			};
		};
		tcpUpdateReadEvent(connection);
	};
	pthread_mutex_unlock(&connection->mutex);
	return 0;
};

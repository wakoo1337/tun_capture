#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "PacketQueueItem.h"
#include "enqueueRxPacket.h"
#include "tcpUpdateReadEvent.h"
#include "processTCPData.h"
#include "HEADERS_RESERVE.h"
#include "MAX_APP_QUEUE.h"

#include "readAndEnqueueSiteData.h"
unsigned int readAndEnqueueSiteData(struct TCPConnection *connection, unsigned int (*on_end)(struct TCPConnection *), unsigned int (*on_error)(struct TCPConnection *)) {
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
				tcpUpdateReadEvent(connection);
				return 0;
			} else if (errno == EINTR) goto data_rx_retry;
			else {
				free(buffer);
				free(item);
				pthread_mutex_lock(&connection->mutex);
				on_error(connection);
				return 0;
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
			on_end(connection);
			return 0;
		};
	};
	tcpUpdateReadEvent(connection);
	return 0;
};

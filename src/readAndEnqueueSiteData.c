#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
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
		struct PacketQueueItem *item;
		item = malloc(sizeof(struct PacketQueueItem));
		if (NULL == item) {
			return 1;
		};
		const unsigned int remain = MAX_APP_QUEUE - connection->app_scheduled;
		const unsigned int to_read = (remain < connection->max_pktdata) ? remain : connection->max_pktdata;
		pthread_mutex_unlock(&connection->mutex);
		uint8_t *buffer;
		buffer = malloc(HEADERS_RESERVE + to_read);
		if (NULL == buffer) {
			free(item);
			return 1;
		};
		ssize_t received;
		data_rx_retry:
		received = recv(connection->sock, &buffer[HEADERS_RESERVE], to_read, 0);
		if (received < 0) {
			if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
				free(buffer);
				free(item);
				pthread_mutex_lock(&connection->mutex);
				const unsigned int result = tcpUpdateReadEvent(connection);
				return result;
			} else if (errno == EINTR) goto data_rx_retry;
			else {
				free(buffer);
				free(item);
				pthread_mutex_lock(&connection->mutex);
				const unsigned int result = on_error(connection);
				return result;
			};
		} else if (received > 0) {
			uint8_t *new_buffer;
			new_buffer = realloc(buffer, HEADERS_RESERVE + received);
			if (NULL == new_buffer) {
				free(buffer);
				return 1;
			};
			item->data = &new_buffer[HEADERS_RESERVE];
			item->count = received;
			item->processor = &processTCPData;
			item->mutex = &connection->mutex;
			item->semaphore = &connection->semaphore;
			item->free_me = new_buffer;
			item->arg = connection;
			enqueueRxPacket(connection->context, item);
			pthread_mutex_lock(&connection->mutex); // Разблокировать не надо, он будет нужен либо на следующей итерации цикла, либо при выходе
			connection->app_scheduled += received;
		} else {
			free(buffer);
			free(item);
			pthread_mutex_lock(&connection->mutex);
			const unsigned int result = on_end(connection);
			return result;
		};
	};
	const unsigned int result = tcpUpdateReadEvent(connection);
	return result;
};

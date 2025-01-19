#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "contrib/logdel_heap.h"
#include "SrcDstSockaddrs.h"
#include "CaptureContext.h"
#include "TCPAppQueueItem.h"
#include "TCPConnection.h"
#include "cancelTimeout.h"
#include "freeNoRefsAppQueueItem.h"
#include "incrementAppQueueItemRefCount.h"
#include "decrementAppQueueItemRefCount.h"

#include "tcpCleanupConfirmed.h"
void tcpCleanupConfirmed(struct TCPConnection *connection) {
	struct TCPAppQueueItem *found = connection->app_queue;
	while (found && found->is_filled && (found->confirm_ack != connection->latest_ack)) {
		found = found->next;
	};
	if (found && found->is_filled) {
		struct TCPAppQueueItem *old_begin = connection->app_queue; // Запоминаем старое начало
		connection->app_queue = found->next; // Делаем следующий элемент началом очереди
		if (NULL == connection->app_queue) connection->app_last = &connection->app_queue; // Если нужно, исправляем указатель на место вставки
		found->next = NULL; // Обнуляем указатель на элемент, следующий за удаляемым, чтобы вовремя остановиться
		struct TCPAppQueueItem *current = old_begin;
		while (current) {
			incrementAppQueueItemRefCount(current);
			const unsigned int new_app_scheduled = connection->app_scheduled - current->data_size;
			connection->app_scheduled = (new_app_scheduled <= connection->app_scheduled) ? new_app_scheduled : 0;
			cancelTimeout(connection->context, &connection->mutex, &current->timeout);
			decrementAppQueueItemRefCount(current);
			decrementAppQueueItemRefCount(current);
			struct TCPAppQueueItem *next = current->next;
			freeNoRefsAppQueueItem(current);
			current = next;
		};
	};
};

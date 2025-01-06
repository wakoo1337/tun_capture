#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPAppQueueItem.h"
#include "TCPConnection.h"
#include "isAppQueueItemInWindow.h"
#include "enqueueTCPPacketTransmission.h"
#include "enqueueTCPRetransmission.h"
#include "freeNoRefsAppQueueItem.h"
#include "incrementAppQueueItemRefCount.h"
#include "decrementAppQueueItemRefCount.h"

#include "enqueueUnsentTCPPacketsTransmission.h"
unsigned int enqueueUnsentTCPPacketsTransmission(struct TCPConnection *connection) {
	struct TCPAppQueueItem *current;
	current = connection->app_queue;
	while (current && current->is_filled) {
		struct TCPAppQueueItem *next;
		next = current->next;
		if ((current->timeout == NULL) && isAppQueueItemInWindow(connection->latest_ack, connection->app_window, current)) {
			incrementAppQueueItemRefCount(current); // Добавляем ещё одну ссылку, так как она у нас в локальной переменной
			if (enqueueTCPPacketTransmission(current) || enqueueTCPRetransmission(current)) return 1; // TODO сделать нормальный способ удаления
			decrementAppQueueItemRefCount(current);
			freeNoRefsAppQueueItem(current);
		};
		current = next;
	};
	return 0;
};

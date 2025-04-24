#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <assert.h>
#include "contrib/avl.h"
#include "SrcDstSockaddrs.h"
#include "TCPAppQueueItem.h"
#include "TCPConnection.h"
#include "isAppQueueItemInWindow.h"
#include "enqueueTCPPacketTransmission.h"
#include "enqueueTCPRetransmission.h"
#include "incrementAppQueueItemRefCount.h"
#include "decrementAppQueueItemRefCount.h"
#include "freeNoRefsAppQueueItem.h"

#include "enqueueUnsentTCPPacketsTransmission.h"
unsigned int enqueueUnsentTCPPacketsTransmission(struct TCPConnection *connection) {
	struct TCPAppQueueItem *found;
	while ((found = avl_find(connection->app_prequeue, &connection->seq_first)) && isAppQueueItemInWindow(connection->latest_ack, connection->app_window, found)) {
		const void *deleted = avl_delete(connection->app_prequeue, found);
		assert(deleted == found);
		connection->seq_first += found->data_size;
		*(connection->app_last) = found;
		connection->app_last = &found->next;
		if (enqueueTCPPacketTransmission(found)) {
		};
		incrementAppQueueItemRefCount(found);
		if (enqueueTCPRetransmission(found)) {
		}; // TODO обработать ошибку, удалить элемент
		decrementAppQueueItemRefCount(found);
		freeNoRefsAppQueueItem(found);
	};
	return 0;
};

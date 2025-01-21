#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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
	struct ItemsLinkedList {
		struct TCPAppQueueItem *item;
		struct ItemsLinkedList *next;
	} *first, **last;
	first = NULL;
	last = &first;
	struct TCPAppQueueItem *current;
	current = connection->app_queue;
	while (current && current->is_filled) {
		if ((current->timeout == NULL) && isAppQueueItemInWindow(connection->latest_ack, connection->app_window, current)) {
			struct ItemsLinkedList *new_item;
			new_item = malloc(sizeof *new_item);
			new_item->item = current;
			incrementAppQueueItemRefCount(new_item->item);
			new_item->next = NULL;
			*last = new_item;
			last = &new_item->next;
		};
		current = current->next;
	};
	while (first) {
		struct ItemsLinkedList *next;
		next = first->next;
		if (enqueueTCPPacketTransmission(first->item) || enqueueTCPRetransmission(first->item)) return 1; // TODO сделать нормальный способ удаления
		decrementAppQueueItemRefCount(first->item);
		freeNoRefsAppQueueItem(first->item);
		free(first);
		first = next;
	};
	return 0;
};

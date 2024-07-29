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
#include "dequeueSiteQueueItem.h"

#include "tcpEstablishedWriteCallback.h"
unsigned int tcpEstablishedWriteCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	if (what & EV_WRITE) {
		struct TCPSiteQueueItem *item;
		item = dequeueSiteQueueItem(connection);
		while (item) {
			ssize_t written;
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
			item = dequeueSiteQueueItem(connection);
		};
	};
	return 0;
};
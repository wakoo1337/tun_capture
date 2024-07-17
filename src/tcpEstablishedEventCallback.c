#include <pthread.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "SrcDstSockaddrs.h"
#include "TCPSiteQueueItem.h"
#include "TCPConnection.h"
#include "dequeueSiteQueueItem.h"
#include "tcpUpdateEvent.h"

#include "tcpEstablishedEventCallback.h"
void tcpEstablishedEventCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	if (what & EV_READ) {
		// TODO сделать чтение данных и их постановку в очередь, обработку не здесь
	};
	if (what & EV_WRITE) {
		struct TCPSiteQueueItem *item;
		while (true) {
			ssize_t written;
			item = dequeueSiteQueueItem(connection);
			if (NULL == item) break;
			if (item->already_sent < item->urgent_count) {
				written = send(connection->sock, item->buffer + item->already_sent, item->urgent_count - item->already_sent, MSG_OOB);
				if (-1 == written) {
					if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) break;
					else {
						// TODO обработать ошибку
					};
				} else {
					item->already_sent += written;
					connection->site_scheduled -= written;
				};
			};
			if (item->already_sent >= item->urgent_count) {
				written = send(connection->sock, item->buffer + item->already_sent, item->urgent_count + item->data_count - item->already_sent, 0);
				if (-1 == written) {
					if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) break;
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

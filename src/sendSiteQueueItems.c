#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "TCPSiteQueueItem.h"
#include "tcpFinalizeRead.h"
#include "tcpFinalizeWrite.h"
#include "tcpstate_connreset.h"

#include "sendSiteQueueItems.h"
unsigned int sendSiteQueueItems(struct TCPConnection *connection) {
	struct TCPSiteQueueItem *item;
	while ((item = connection->site_queue)) {
		if (item->already_sent < item->urgent_count) {
			ssize_t written;
			urgent_tx_retry:
			written = send(connection->sock, item->buffer + item->already_sent, item->urgent_count - item->already_sent, MSG_OOB | MSG_NOSIGNAL);
			if (-1 == written) {
				switch (errno) {
					case EINTR:
						goto urgent_tx_retry;
					break;
					case ECONNABORTED:
					case EHOSTUNREACH:
					case ECONNREFUSED:
					case ECONNRESET:
					case EPIPE:
					case ETIMEDOUT:
					case EINVAL:
						connection->state = &tcpstate_connreset;
						tcpFinalizeRead(connection);
						tcpFinalizeWrite(connection);
					case ESHUTDOWN:
						return 0;
					break;
					default:
						if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) return 0;
						else return 1;
				};
			} else {
				item->already_sent += written;
				connection->site_scheduled -= written;
			};
		};
		if (item->already_sent >= item->urgent_count) {
			ssize_t written;
			data_tx_retry:
			written = send(connection->sock, item->buffer + item->already_sent, item->urgent_count + item->data_count - item->already_sent, MSG_NOSIGNAL);
			if (-1 == written) {
				switch (errno) {
					case EINTR:
						goto data_tx_retry;
					break;
					case ECONNABORTED:
					case EHOSTUNREACH:
					case ECONNREFUSED:
					case ECONNRESET:
					case EPIPE:
					case ETIMEDOUT:
						connection->state = &tcpstate_connreset;
						tcpFinalizeRead(connection);
						tcpFinalizeWrite(connection);
					case ESHUTDOWN:
						return 0;
					break;
					default:
						if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) return 0;
						else return 1;
				};
			} else {
				item->already_sent += written;
				connection->site_scheduled -= written;
			};
		};
		if (item->already_sent == item->urgent_count + item->data_count) {
			struct TCPSiteQueueItem *next;
			next = item->next;
			free(item->free_me);
			free(item);
			connection->site_queue = next;
		};
	};
	if (NULL == connection->site_queue) connection->site_last = &connection->site_queue;
	return 0;
};

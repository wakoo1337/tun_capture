#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "TCPSiteQueueItem.h"
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "sendSiteQueueItems.h"
#include "tcpFinalizeWrite.h"
#include "tcpUpdateWriteEvent.h"

#include "tcpGotFINWriteCallback.h"
unsigned int tcpGotFINWriteCallback(evutil_socket_t fd, short what, void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	if (what & EV_WRITE) {
		sendSiteQueueItems(connection);
		if (NULL == connection->site_queue) {
			// Все данные на сайт отправлены, а новых не будет
			if (!connection->write_finalized) {
				shutdown(connection->sock, SHUT_WR);
				tcpFinalizeWrite(connection);
			};
			event_add(connection->read_event, NULL);
		} else tcpUpdateWriteEvent(connection);
	};
	pthread_mutex_unlock(&connection->mutex);
	return 0;
};

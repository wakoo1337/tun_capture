#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <event2/event.h>
#include "contrib/avl.h"
#include "CaptureSettings.h"
#include "CaptureContext.h"
#include "IPPacketPayload.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "SrcDstSockaddrs.h"
#include "TCPHeaderData.h"
#include "TCPConnection.h"
#include "TCPState.h"
#include "parseTCPHeader.h"
#include "compareTCPSitePrequeueItems.h"
#include "tcpReadCallback.h"
#include "tcpWriteCallback.h"
#include "tcpFinalizeRead.h"
#include "tcpFinalizeWrite.h"
#include "compareTCPAppPrequeueItems.h"
#include "tcpDestroySitePrequeue.h"
#include "tcpDestroyAppPrequeue.h"
#include "sendTCPReset.h"
#include "tcpstate_connwait.h"
#include "tcpstate_connreset.h"

#include "processTCPPacket.h"
unsigned int processTCPPacket(struct CaptureContext *context, const struct IPPacketPayload *payload, const struct NetworkProtocolStrategy *strategy, struct SrcDstSockaddrs *addrs) {
	struct TCPHeaderData hdr;
	if (parseTCPHeader(&hdr, payload->packet, payload->count, payload->pseudo, strategy->pseudo_length)) {
		free(payload->free_me);
		return 0;
	};
	strategy->port_setter(&addrs->src, hdr.src_port);
	strategy->port_setter(&addrs->dst, hdr.dst_port);
	if (hdr.rst) {
		// Удалить соединение
		pthread_mutex_lock(&context->tcp_mutex);
		struct TCPConnection *found;
		found = avl_find(context->tcp_connections, addrs);
		if (found) {
			pthread_mutex_lock(&found->mutex);
			pthread_mutex_unlock(&context->tcp_mutex);
			found->state = &tcpstate_connreset;
			tcpFinalizeRead(found);
			tcpFinalizeWrite(found);
			pthread_mutex_unlock(&found->mutex);
		} else pthread_mutex_unlock(&context->tcp_mutex);
		free(payload->free_me);
		return 0;
	};
	if ((!hdr.urg) && (!hdr.ack) && (!hdr.psh) && (!hdr.rst) && hdr.syn && (!hdr.fin)) {
		// Установить новое соединение
		struct TCPConnection *connection;
		assert(addrs->src.sa_family == addrs->dst.sa_family);
		connection = malloc(sizeof(struct TCPConnection));
		if (NULL == connection) {
			free(payload->free_me);
			return 1;
		};
		connection->addrs = *addrs;
		pthread_mutex_lock(&context->tcp_mutex);
		void **probe;
		probe = avl_probe(context->tcp_connections, connection);
		if (NULL == probe) {
			// Невозможно вставить
			free(payload->free_me);
			pthread_mutex_unlock(&context->tcp_mutex);
			free(connection);
			return 1;
		} else if (*probe != ((void *) connection)) {
			// Уже есть такое соединение
			free(payload->free_me);
			pthread_mutex_unlock(&context->tcp_mutex);
			free(connection);
			return 0;
		};
		pthread_mutex_init(&connection->mutex, NULL);
		sem_init(&connection->semaphore, 0, context->settings->threads_count);
		sem_wait(&connection->semaphore);
		pthread_mutex_lock(&connection->mutex);
		if (hdr.mss_present) {
			connection->max_pktdata = hdr.mss_value; // Это без учёта дополнительных опций, которые могут ещё появиться
		} else {
			if (addrs->src.sa_family == AF_INET) connection->max_pktdata = 536;
			else if (addrs->src.sa_family == AF_INET6) connection->max_pktdata = 1220;
		};
		connection->site_queue = NULL;
		connection->site_last = &connection->site_queue;
		connection->app_queue = NULL;
		connection->app_last = &connection->app_queue;
		connection->site_scheduled = connection->app_scheduled = 0;
		connection->seq_next = connection->seq_first = 0;
		connection->first_desired = hdr.seq_num + 1;
		connection->scaling_enabled = hdr.winscale_present;
		connection->remote_scale = hdr.winscale_value;
		connection->our_scale = 0; // TODO сделать масштабирование
		connection->read_event = connection->write_event = NULL;
		connection->state = &tcpstate_connwait;
		connection->strategy = strategy;
		connection->context = context;
		connection->should_send_fin = false;
		connection->fin_seq = 0;
		connection->timewait_item = NULL;
		connection->site_prequeue = avl_create(&compareTCPSitePrequeueItems, NULL, NULL);
		if (NULL == connection->site_prequeue) {
			void *deleted;
			deleted = avl_delete(context->tcp_connections, connection);
			assert(deleted == connection);
			pthread_mutex_unlock(&connection->mutex);
			sem_post(&connection->semaphore);
			pthread_mutex_destroy(&connection->mutex);
			pthread_mutex_unlock(&context->tcp_mutex);
			sem_destroy(&connection->semaphore);
			free(connection);
			free(payload->free_me);
			return 1;
		};
		connection->app_prequeue = avl_create(&compareTCPAppPrequeueItems, NULL, NULL);
		if (NULL == connection->app_prequeue) {
			tcpDestroySitePrequeue(connection);
			void *deleted;
			deleted = avl_delete(context->tcp_connections, connection);
			assert(deleted == connection);
			pthread_mutex_unlock(&connection->mutex);
			sem_post(&connection->semaphore);
			pthread_mutex_destroy(&connection->mutex);
			pthread_mutex_unlock(&context->tcp_mutex);
			sem_destroy(&connection->semaphore);
			free(connection);
			free(payload->free_me);
			return 1;
		};
		connection->sock = socket(addrs->src.sa_family, SOCK_STREAM, 0);
		if (-1 == connection->sock) {
			tcpDestroySitePrequeue(connection);
			tcpDestroyAppPrequeue(connection);
			void *deleted;
			deleted = avl_delete(context->tcp_connections, connection);
			assert(deleted == connection);
			pthread_mutex_unlock(&connection->mutex);
			sem_post(&connection->semaphore);
			pthread_mutex_destroy(&connection->mutex);
			pthread_mutex_unlock(&context->tcp_mutex);
			sem_destroy(&connection->semaphore);
			free(connection);
			free(payload->free_me);
			return 1;
		};
		if (-1 == fcntl(connection->sock, F_SETFL, O_NONBLOCK)) {
			tcpDestroySitePrequeue(connection);
			tcpDestroyAppPrequeue(connection);
			void *deleted;
			deleted = avl_delete(context->tcp_connections, connection);
			assert(deleted == connection);
			pthread_mutex_unlock(&connection->mutex);
			sem_post(&connection->semaphore);
			pthread_mutex_destroy(&connection->mutex);
			pthread_mutex_unlock(&context->tcp_mutex);
			sem_destroy(&connection->semaphore);
			free(connection);
			free(payload->free_me);
			return 1;
		};
		connection->read_event = event_new(context->event_base, connection->sock, EV_READ | EV_PERSIST | EV_FINALIZE, &tcpReadCallback, connection);
		if (NULL == connection->read_event) {
			tcpDestroySitePrequeue(connection);
			tcpDestroyAppPrequeue(connection);
			void *deleted;
			deleted = avl_delete(context->tcp_connections, connection);
			assert(deleted == connection);
			pthread_mutex_unlock(&connection->mutex);
			sem_post(&connection->semaphore);
			pthread_mutex_destroy(&connection->mutex);
			pthread_mutex_unlock(&context->tcp_mutex);
			sem_destroy(&connection->semaphore);
			free(connection);
			free(payload->free_me);
			return 1;
		};
		connection->write_event = event_new(context->event_base, connection->sock, EV_WRITE | EV_PERSIST | EV_FINALIZE, &tcpWriteCallback, connection);
		if (NULL == connection->write_event) {
			tcpFinalizeRead(connection);
			connection->state = &tcpstate_connreset;
			void *deleted;
			deleted = avl_delete(context->tcp_connections, connection);
			assert(deleted == connection);
			pthread_mutex_unlock(&connection->mutex);
			sem_post(&connection->semaphore);
			pthread_mutex_destroy(&connection->mutex);
			pthread_mutex_unlock(&context->tcp_mutex);
			sem_destroy(&connection->semaphore);
			free(connection);
			free(payload->free_me);
			return 1;
		};
		if (-1 == event_add(connection->write_event, NULL)) {
			tcpFinalizeRead(connection);
			tcpFinalizeWrite(connection);
			connection->state = &tcpstate_connreset;
			void *deleted;
			deleted = avl_delete(context->tcp_connections, connection);
			assert(deleted == connection);
			pthread_mutex_unlock(&connection->mutex);
			sem_post(&connection->semaphore);
			pthread_mutex_destroy(&connection->mutex);
			pthread_mutex_unlock(&context->tcp_mutex);
			sem_destroy(&connection->semaphore);
			free(connection);
			free(payload->free_me);
			return 1;
		};
		errno = 0;
		if ((-1 == connect(connection->sock, &addrs->dst, sizeof(struct sockaddr))) && (errno != EINPROGRESS)) {
			tcpFinalizeRead(connection);
			tcpFinalizeWrite(connection);
			connection->state = &tcpstate_connreset;
			void *deleted;
			deleted = avl_delete(context->tcp_connections, connection);
			assert(deleted == connection);
			pthread_mutex_unlock(&connection->mutex);
			sem_post(&connection->semaphore);
			pthread_mutex_destroy(&connection->mutex);
			pthread_mutex_unlock(&context->tcp_mutex);
			sem_destroy(&connection->semaphore);
			free(connection);
			free(payload->free_me);
			return 1;
		};
		pthread_mutex_unlock(&context->tcp_mutex);
		pthread_mutex_unlock(&connection->mutex);
		sem_post(&connection->semaphore);
		free(payload->free_me); // TODO убрать, и в первом пакете могут быть данные
		return 0;
	};
	// Попытаться обнаружить соединение и обработать пакет его обработчиком. Если соединение найти не удастся, послать RST
	pthread_mutex_lock(&context->tcp_mutex);
	struct TCPConnection *connection;
	connection = avl_find(context->tcp_connections, addrs);
	if (connection) {
		sem_wait(&connection->semaphore);
		pthread_mutex_lock(&connection->mutex);
		pthread_mutex_unlock(&context->tcp_mutex);
		unsigned int result;
		result = connection->state->packets_processor(connection, payload, &hdr);
		pthread_mutex_unlock(&connection->mutex);
		sem_post(&connection->semaphore);
		return result;
	} else {
		pthread_mutex_unlock(&context->tcp_mutex);
		free(payload->free_me);
		return sendTCPReset(context, payload, strategy, addrs);
	};
	return 0;
};

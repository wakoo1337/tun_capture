#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <event2/event.h>
#include "contrib/avl.h"
#include "contrib/logdel_heap.h"
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
#include "tcpDestroySitePrequeueItem.h"
#include "tcpFinalizeRead.h"
#include "tcpFinalizeWrite.h"
#include "tcpstate_connwait.h"

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
		free(payload->free_me);
		return 0;
	};
	if (hdr.syn) {
		// Установить новое соединение
		struct TCPConnection *connection;
		assert(addrs->src.sa_family == addrs->dst.sa_family);
		connection = malloc(sizeof(struct TCPConnection));
		if (NULL == connection) {
			free(payload->free_me);
			return 1;
		};
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
		connection->site_prequeue = avl_create(&compareTCPSitePrequeueItems, NULL, NULL);
		if (NULL == connection->site_prequeue) {
			free(connection);
			free(payload->free_me);
			return 1;
		};
		connection->site_scheduled = connection->app_scheduled = 0;
		connection->our_seq = 0;
		connection->first_desired = hdr.seq_num + 1;
		connection->scaling_enabled = hdr.winscale_present;
		connection->remote_scale = hdr.winscale_value;
		connection->our_scale = 0; // TODO сделать масштабирование
		connection->addrs = *addrs;
		connection->sock = socket(addrs->src.sa_family, SOCK_STREAM, 0);
		pthread_mutex_init(&connection->mutex, NULL);
		pthread_mutex_lock(&connection->mutex);
		if (-1 == connection->sock) {
			avl_destroy(connection->site_prequeue, &tcpDestroySitePrequeueItem);
			pthread_mutex_unlock(&connection->mutex);
			pthread_mutex_destroy(&connection->mutex);
			free(connection);
			free(payload->free_me);
			return 1;
		};
		if (-1 == fcntl(connection->sock, F_SETFL, O_NONBLOCK)) {
			avl_destroy(connection->site_prequeue, &tcpDestroySitePrequeueItem);
			pthread_mutex_unlock(&connection->mutex);
			pthread_mutex_destroy(&connection->mutex);
			close(connection->sock);
			free(connection);
			free(payload->free_me);
			return 1;
		};
		pthread_mutex_unlock(&connection->mutex);
		pthread_mutex_lock(&context->tcp_mutex);
		pthread_mutex_lock(&connection->mutex);
		connection->read_finalized = connection->write_finalized = false;
		connection->state = &tcpstate_connwait;
		connection->strategy = strategy;
		connection->context = context;
		connection->read_event = event_new(context->event_base, connection->sock, EV_READ | EV_PERSIST | EV_FINALIZE, &tcpReadCallback, connection);
		if (NULL == connection->read_event) {
			pthread_mutex_unlock(&context->tcp_mutex);
			pthread_mutex_unlock(&connection->mutex);
			pthread_mutex_destroy(&connection->mutex);
			avl_destroy(connection->site_prequeue, &tcpDestroySitePrequeueItem);
			close(connection->sock);
			free(connection);
			free(payload->free_me);
			return 1;
		};
		connection->write_event = event_new(context->event_base, connection->sock, EV_WRITE | EV_PERSIST | EV_FINALIZE, &tcpWriteCallback, connection);
		if (NULL == connection->write_event) {
			connection->write_finalized = true;
			event_free_finalize(0, connection->read_event, &tcpFinalizeRead);
			pthread_mutex_unlock(&context->tcp_mutex);
			pthread_mutex_unlock(&connection->mutex);
			free(payload->free_me);
			return 1;
		};
		if (-1 == event_add(connection->write_event, NULL)) {
			event_free_finalize(0, connection->read_event, &tcpFinalizeRead);
			event_free_finalize(0, connection->write_event, &tcpFinalizeWrite);
			pthread_mutex_unlock(&context->tcp_mutex);
			pthread_mutex_unlock(&connection->mutex);
			free(payload->free_me);
			return 1;
		};
		errno = 0;
		if ((-1 == connect(connection->sock, &addrs->dst, sizeof(struct sockaddr))) && (errno != EINPROGRESS)) {
			event_free_finalize(0, connection->read_event, &tcpFinalizeRead);
			event_free_finalize(0, connection->write_event, &tcpFinalizeWrite);
			pthread_mutex_unlock(&context->tcp_mutex);
			pthread_mutex_unlock(&connection->mutex);
			free(payload->free_me);
			return 1;
		};
		void **probe;
		probe = avl_probe(context->tcp_connections, connection);
		if ((NULL == probe) || ((*probe) != connection)) {
			event_free_finalize(0, connection->read_event, &tcpFinalizeRead);
			event_free_finalize(0, connection->write_event, &tcpFinalizeWrite);
			pthread_mutex_unlock(&context->tcp_mutex);
			pthread_mutex_unlock(&connection->mutex);
			free(payload->free_me);
			return (unsigned int) (NULL == probe);
		};
		pthread_mutex_unlock(&context->tcp_mutex);
		pthread_mutex_unlock(&connection->mutex);
		free(payload->free_me); // TODO убрать, и в первом пакете могут быть данные
		return 0;
	};
	// Попытаться обнаружить соединение и обработать пакет его обработчиком. Если соединение найти не удастся, послать RST
	pthread_mutex_lock(&context->tcp_mutex);
	struct TCPConnection *connection;
	connection = avl_find(context->tcp_connections, addrs);
	if (connection) {
		pthread_mutex_lock(&connection->mutex);
		pthread_mutex_unlock(&context->tcp_mutex);
		unsigned int result;
		result = connection->state->packets_processor(connection, payload, &hdr);
		pthread_mutex_unlock(&connection->mutex);
		return result;
	} else {
		// TODO послать RST
		pthread_mutex_unlock(&context->tcp_mutex);
		free(payload->free_me);
	};
	return 0;
};

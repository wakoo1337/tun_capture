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
#include "contrib/heap.h"
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
#include "tcpstate_connwait.h"

#include "processTCPPacket.h"
unsigned int processTCPPacket(struct CaptureContext *context, const struct IPPacketPayload *payload, const struct NetworkProtocolStrategy *strategy, struct SrcDstSockaddrs *addrs) {
	struct TCPHeaderData hdr;
	if (parseTCPHeader(&hdr, payload->packet, payload->count, payload->pseudo, strategy->pseudo_length)) return 1;
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
		if (NULL == connection) return 1;
		connection->addrs = *addrs;
		connection->sock = socket(addrs->src.sa_family, SOCK_STREAM, 0);
		if (-1 == connection->sock) {
			free(connection);
			return 1;
		};
		if (-1 == fcntl(connection->sock, F_SETFL, O_NONBLOCK)) {
			close(connection->sock);
			free(connection);
			return 1;
		};
		errno = 0;
		if ((-1 == connect(connection->sock, &addrs->dst, sizeof(struct sockaddr))) && (errno != EINPROGRESS)) {
			close(connection->sock);
			free(connection);
			return 1;
		};
		pthread_mutex_lock(&context->tcp_mutex);
		pthread_mutex_init(&connection->mutex, NULL);
		pthread_mutex_lock(&connection->mutex);
		connection->state = &tcpstate_connwait;
		connection->strategy = strategy;
		connection->context = context;
		connection->read_event = event_new(context->event_base, connection->sock, EV_READ | EV_PERSIST, &tcpReadCallback, connection);
		if (NULL == connection->read_event) {
			pthread_mutex_unlock(&context->tcp_mutex);
			pthread_mutex_unlock(&connection->mutex);
			pthread_mutex_destroy(&connection->mutex);
			close(connection->sock);
			free(connection);
			return 1;
		};
		connection->write_event = event_new(context->event_base, connection->sock, EV_WRITE | EV_PERSIST, &tcpWriteCallback, connection);
		if (NULL == connection->write_event) {
			event_free(connection->read_event);
			pthread_mutex_unlock(&context->tcp_mutex);
			pthread_mutex_unlock(&connection->mutex);
			pthread_mutex_destroy(&connection->mutex);
			close(connection->sock);
			free(connection);
			return 1;
		};
		if (-1 == event_add(connection->write_event, NULL)) {
			event_free(connection->write_event);
			event_free(connection->read_event);
			pthread_mutex_unlock(&context->tcp_mutex);
			pthread_mutex_unlock(&connection->mutex);
			pthread_mutex_destroy(&connection->mutex);
			close(connection->sock);
			free(connection);
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
		connection->site_scheduled = connection->app_scheduled = 0;
		connection->our_seq = 0;
		connection->first_desired = hdr.seq_num + 1;
		connection->scaling_enabled = hdr.winscale_present;
		connection->remote_scale = hdr.winscale_value;
		connection->our_scale = 0; // TODO сделать масштабирование
		void **probe;
		probe = avl_probe(context->tcp_connections, connection);
		if ((NULL == probe) || ((*probe) != connection)) {
			event_free(connection->write_event);
			event_free(connection->read_event);
			pthread_mutex_unlock(&context->tcp_mutex);
			pthread_mutex_unlock(&connection->mutex);
			pthread_mutex_destroy(&connection->mutex);
			close(connection->sock);
			free(connection);
			return 1;
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
	};
	return 0;
};

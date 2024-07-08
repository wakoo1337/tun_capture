#include <pthread.h>
#include <assert.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <event2/event.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "contrib/avl.h"
#include "CaptureContext.h"
#include "IPPacketPayload.h"
#include "IPFragmentMetadata.h"
#include "NetworkProtocolStrategy.h"
#include "SrcDstSockaddrs.h"
#include "TCPHeaderData.h"
#include "TCPState.h"
#include "TCPConnection.h"
#include "parseTCPHeader.h"
#include "tcpCallback.h"
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
		if ((-1 == connect(connection->sock, &addrs->dst, sizeof(struct sockaddr))) && (errno != EINPROGRESS)) {
			close(connection->sock);
			free(connection);
			return 1;
		};
		pthread_mutex_init(&connection->mutex, NULL);
		connection->state = &tcpstate_connwait;
		connection->strategy = strategy;
		connection->context = context;
		connection->event = event_new(context->event_base, connection->sock, EV_WRITE, &tcpCallback, connection);
		if (NULL == connection->event) {
			pthread_mutex_destroy(&connection->mutex);
			close(connection->sock);
			free(connection);
			return 1;
		};
		if (-1 == event_add(connection->event, NULL)) {
			pthread_mutex_destroy(&connection->mutex);
			close(connection->sock);
			free(connection);
			return 1;
		};
		pthread_mutex_lock(&context->tcp_mutex);
		void **probe;
		probe = avl_probe(context->tcp_connections, connection);
		if ((NULL == probe) || ((*probe) != connection)) {
			pthread_mutex_destroy(&connection->mutex);
			close(connection->sock);
			free(connection);
			pthread_mutex_unlock(&context->tcp_mutex);
			return 1;
		};
		pthread_mutex_unlock(&context->tcp_mutex);
		return 0;
	};
	// Попытаться обнаружить соединение и обработать пакет его обработчиком. Если соединение найти не удастся, послать RST
	return 0;
};

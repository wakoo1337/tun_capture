#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <string.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "compareTCPConnections.h"
int compareTCPConnections(const void *a, const void *b, void *futile) {
	const struct TCPConnection *a_conn = (const struct TCPConnection *) a;
	const struct TCPConnection *b_conn = (const struct TCPConnection *) b;
	return memcmp(&a_conn->addrs, &b_conn->addrs, sizeof(struct SrcDstSockaddrs));
};

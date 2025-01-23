#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"

#include "scaleRemoteWindow.h"
unsigned int scaleRemoteWindow(struct TCPConnection *connection, uint16_t raw) {
	return ((unsigned int) raw) << (connection->scaling_enabled ? connection->remote_scale : 0);
};

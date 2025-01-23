#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <semaphore.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPConnection.h"
#include "tcpFinalizeRead.h"
#include "tcpFinalizeWrite.h"

#include "tcpTimeWaitExpiredCallback.h"
void tcpTimeWaitExpiredCallback(void *arg) {
	struct TCPConnection *connection = (struct TCPConnection *) arg;
	connection->timewait_item = NULL;
	tcpFinalizeRead(connection); // Вообще, если соединение в состоянии TimeWait, то событие чтения уже уничтожено
	tcpFinalizeWrite(connection);
};

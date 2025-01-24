#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "TCPAppQueueItem.h"
#include "enqueueTCPPacketTransmission.h"
#include "enqueueTCPRetransmission.h"

#include "tcpRetransmissionTimerCallback.h"
void tcpRetransmissionTimerCallback(void *arg) {
	struct TCPAppQueueItem *item;
	item = (struct TCPAppQueueItem *) arg;
	enqueueTCPPacketTransmission(item);
	item->timeout = NULL;
	enqueueTCPRetransmission(item);
};

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "SrcDstSockaddrs.h"
#include "TCPSiteQueueItem.h"
#include "TCPConnection.h"
#include "MAX_SITE_QUEUE.h"

#include "getSendWindowSize.h"
uint16_t getSendWindowSize(struct TCPConnection *conn) {
	unsigned int queue_sum = 0;
	struct TCPSiteQueueItem *current;
	current = conn->site_queue;
	while (current) {
		queue_sum += current->urgent_count + current->data_count - current->already_sent;
		current = current->next;
	};
	if (queue_sum > MAX_SITE_QUEUE) return 0;
	else {
		const unsigned int scaled = (MAX_SITE_QUEUE - queue_sum) >> (conn->scaling_enabled ? conn->our_scale : 0);
		return (scaled > 65535) ? 65535 : scaled;
	};
};

#include <stdint.h>
#include "TCPAppQueueItem.h"

#include "compareTCPAppPrequeueItems.h"
int compareTCPAppPrequeueItems(const void *a_void, const void *b_void, void *futile) {
	const struct TCPAppQueueItem *a = (const struct TCPAppQueueItem *) a_void;
	const struct TCPAppQueueItem *b = (const struct TCPAppQueueItem *) b_void;
	if (a->seq > b->seq) return 1;
	else if (a->seq < b->seq) return -1;
	else return 0;
};

#include <stdint.h>
#include <stdbool.h>
#include "TCPSitePrequeueItem.h"

#include "compareTCPSitePrequeueItems.h"
int compareTCPSitePrequeueItems(const void *a, const void *b, void *futile) {
	const struct TCPSitePrequeueItem *a_item = (const struct TCPSitePrequeueItem *) a;
	const struct TCPSitePrequeueItem *b_item = (const struct TCPSitePrequeueItem *) b;
	if (a_item->seq == b_item->seq) return 0;
	else if (a_item->seq > b_item->seq) return 1;
	else return -1;
};

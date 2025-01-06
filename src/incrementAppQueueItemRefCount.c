#include <stdint.h>
#include <stdbool.h>
#include "TCPAppQueueItem.h"

#include "incrementAppQueueItemRefCount.h"
void incrementAppQueueItemRefCount(struct TCPAppQueueItem *item) {
	item->ref_count = (((unsigned int) -1) == item->ref_count) ? item->ref_count : (item->ref_count+1);
};

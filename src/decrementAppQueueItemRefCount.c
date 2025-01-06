#include <stdint.h>
#include <stdbool.h>
#include "TCPAppQueueItem.h"

#include "decrementAppQueueItemRefCount.h"
void decrementAppQueueItemRefCount(struct TCPAppQueueItem *item) {
	item->ref_count = (0 == item->ref_count) ? item->ref_count : (item->ref_count-1);
};

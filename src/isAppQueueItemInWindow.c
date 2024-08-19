#include <stdint.h>
#include <stdbool.h>
#include "TCPAppQueueItem.h"
#include "checkByteInWindow.h"

#include "isAppQueueItemInWindow.h"
bool isAppQueueItemInWindow(uint32_t ack, unsigned int window, struct TCPAppQueueItem *item) {
	return checkByteInWindow(ack, window, item->confirm_ack - item->data_size) && checkByteInWindow(ack, window, item->confirm_ack);
};

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>
#include "CaptureContext.h"
#include "TimeoutItem.h"
#include "compareTimeval.h"

#include "compareTimeoutItems.h"
bool compareTimeoutItems(const void *a_data, const void *b_data, void *) {
	const struct TimeoutItem *a = (const struct TimeoutItem *) a_data;
	const struct TimeoutItem *b = (const struct TimeoutItem *) b_data;
	return compareTimeval(&b->expiration, &a->expiration) > 0; // Элементы поменяны местами, так как важнее тот, у которого время устаревания меньше
};

#include <stdbool.h>
#include <sys/time.h>
#include "TimeoutItem.h"
#include "compareTimeval.h"

#include "compareTimeoutItems.h"
int compareTimeoutItems(const void *a_data, const void *b_data, const void *) {
	const struct TimeoutItem *a = (const struct TimeoutItem *) a_data;
	const struct TimeoutItem *b = (const struct TimeoutItem *) b_data;
	return compareTimeval(&a->expiration, &b->expiration);
};

#include <stdbool.h>
#include <sys/time.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "TimeoutItem.h"
#include "compareTimeval.h"

bool compareTimeoutItems(pq_data_t a_data, pq_data_t b_data) {
	const struct TimeoutItem *a = (const struct TimeoutItem *) a_data;
	const struct TimeoutItem *b = (const struct TimeoutItem *) b_data;
	return compareTimeval(&a->expiration, &b->expiration) < 0;
};

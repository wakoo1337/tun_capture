#include <stdbool.h>
#include <time.h>
#include <netinet/in.h>
#include "contrib/C-Collections/pqlib/PQ.h"
#include "IPv4Fragments.h"
#include "compareTimespec.h"

#include "compareIPv4Queue.h"
bool compareIPv4Queue(pq_data_t a, pq_data_t b) {
	return compareTimespec(&((struct IPv4Fragments *) a)->expiration, &((struct IPv4Fragments *) b)->expiration) > 0;
};

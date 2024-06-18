#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <netinet/in.h>
#include "IPv4Fragments.h"

#include "compareIPv4FragmentsIdsSources.h"
int compareIPv4FragmentsIdsSources(const void *a, const void *b, void *futile) {
	const struct IPv4Fragments *frag_a = (const struct IPv4Fragments *) a;
	const struct IPv4Fragments *frag_b = (const struct IPv4Fragments *) b;
	
	if (frag_a->id == frag_b->id) return memcmp(&frag_a->src, &frag_b->src, sizeof(struct in_addr));
	else if (frag_a->id > frag_b->id) return 1;
	else return -1;
};

#include <semaphore.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <event2/event.h>
#include "UDPBinding.h"

#include "compareUDPBindings.h"
int compareUDPBindings(const void *a, const void *b, void *futile) {
	const struct UDPBinding *a_bind = (const struct UDPBinding *) a;
	const struct UDPBinding *b_bind = (const struct UDPBinding *) b;
	return memcmp(&(a_bind->internal_addr), &(b_bind->internal_addr), sizeof(struct sockaddr));
};

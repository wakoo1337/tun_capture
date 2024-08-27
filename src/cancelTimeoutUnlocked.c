#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include "TimeoutItem.h"

#include "cancelTimeoutUnlocked.h"
void cancelTimeoutUnlocked(struct TimeoutItem **item) {
	if (NULL != (*item)) {
		(*item)->is_del = true;
		*item = NULL;
	};
};

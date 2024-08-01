struct TimeoutItem *enqueueTimeout(struct CaptureContext *context, const struct timeval *timeout, void (*callback)(void *), void *arg, pthread_mutex_t *mutex);

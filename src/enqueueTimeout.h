struct TimeoutItem *enqueueTimeout(struct CaptureContext *context, const struct timeval *timeout, bool (*callback)(struct CaptureContext *, struct TimeoutItem *), void *arg);

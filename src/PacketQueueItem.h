struct PacketQueueItem {
	struct RefcountBuffer *buffer;
	unsigned int (*processor)(struct CaptureContext *, struct RefcountBuffer *, void *);
	pthread_mutex_t *mutex;
	void *arg;
	struct PacketQueueItem *next;
};

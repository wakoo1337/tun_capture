struct PacketQueueItem {
	uint8_t *data;
	unsigned int count;
	unsigned int (*processor)(struct CaptureContext *, uint8_t *, unsigned int, void *);
	pthread_mutex_t *mutex;
	void *free_me, *arg;
	struct PacketQueueItem *next;
};

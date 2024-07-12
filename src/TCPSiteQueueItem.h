struct TCPSiteQueueItem {
	uint8_t *buffer;
	unsigned int urgent_count;
	unsigned int data_count;
	unsigned int already_sent;
	void *free_me;
	struct TCPSiteQueueItem *next;
};

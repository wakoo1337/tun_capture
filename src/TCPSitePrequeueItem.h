struct TCPSitePrequeueItem {
	uint32_t seq;
	uint8_t *data;
	unsigned int urgent_count, data_count;
	struct TCPConnection *connection;
	struct TimeoutItem *timeout;
	void *free_me;
	bool fin;
};

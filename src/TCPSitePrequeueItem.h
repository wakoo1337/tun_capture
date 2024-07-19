struct TCPSitePrequeueItem {
	uint32_t seq;
	uint8_t *data;
	unsigned int urgent_count, data_count;
	uint32_t packet_ack;
	unsigned int window;
	struct TCPConnection *connection;
	struct TimeoutItem *timeout;
	void *free_me;
};

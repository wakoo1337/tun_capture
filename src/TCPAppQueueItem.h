struct TCPAppQueueItem {
	uint8_t *ip_packet;
	unsigned int ip_size;
	unsigned int data_size;
	uint32_t confirm_ack;
	struct TCPConnection *connection;
	struct TimeoutItem *timeout;
	void *free_me;
	bool is_filled;
	unsigned int ref_count;
	struct TCPAppQueueItem *next;
};

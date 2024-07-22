struct TCPAppQueueItem {
	uint8_t *ip_packet;
	unsigned int ip_size;
	uint8_t *tcp_packet;
	unsigned int tcp_size;
	unsigned int data_size;
	uint32_t confirm_ack;
	struct TCPConnection *connection;
	struct TimeoutItem *timeout;
	void *free_me;
	struct TCPAppQueueItem *next;
};

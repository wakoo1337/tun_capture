struct TCPAppQueueItem {
	uint8_t *ip_packet;
	unsigned int ip_size;
	uint8_t *tcp_packet;
	unsigned int tcp_size;
	struct timeval retry;
	uint32_t confirm_ack;
	void *free_me;
	struct TCPAppQueueItem *next;
};

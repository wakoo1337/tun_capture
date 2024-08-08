struct TCPAppQueueItem {
	struct RefcountBuffer *buffer;
	uint32_t confirm_ack;
	struct TCPConnection *connection;
	struct TimeoutItem *timeout;
	void *free_me;
	bool is_filled;
	struct TCPAppQueueItem *next;
};

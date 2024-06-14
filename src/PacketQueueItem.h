struct PacketQueueItem {
	void *data;
	unsigned int count;
	struct PacketQueueItem *next;
};

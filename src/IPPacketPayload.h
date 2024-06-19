struct IPPacketPayload {
	void *free_me;
	uint8_t *packet, *pseudo;
	unsigned int count;
};

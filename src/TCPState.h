struct TCPState {
	void (*event_callback)(evutil_socket_t, short, void *);
	unsigned int (*packets_processor)(const struct IPPacketPayload *, const struct TCPHeaderData *);
};
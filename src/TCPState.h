struct TCPState {
	unsigned int (*read_callback)(evutil_socket_t, short, void *);
	unsigned int (*write_callback)(evutil_socket_t, short, void *);
	unsigned int (*packets_processor)(struct TCPConnection *, const struct IPPacketPayload *, const struct TCPHeaderData *);
};

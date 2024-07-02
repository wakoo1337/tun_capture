struct NetworkProtocolStrategy {
	unsigned int pseudo_length;
	void (*port_setter)(struct sockaddr *, uint16_t);
	uint16_t (*port_getter)(const struct sockaddr *);
	unsigned int (*compute_fragcount)(unsigned int, unsigned int);
	void (*fill_metadatas)(struct IPFragmentMetadata *, unsigned int, unsigned int, const unsigned int);
	void (*create_pseudo)(void *, const struct sockaddr *, const struct sockaddr *, uint8_t, unsigned int);
	void (*write_headers)(struct CaptureContext *, struct IPFragmentMetadata *, unsigned int, uint8_t, const struct sockaddr *, const struct sockaddr *);
};

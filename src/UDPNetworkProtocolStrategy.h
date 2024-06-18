struct UDPNetworkProtocolStrategy {
	unsigned int pseudo_length;
	void (*port_setter)(struct sockaddr *sa, uint16_t port);
};

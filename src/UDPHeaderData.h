struct UDPHeaderData {
	uint16_t src_port, dst_port;
	unsigned int length; // Длина с заголовком!
	uint16_t checksum;
};

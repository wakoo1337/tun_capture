struct RefcountBuffer {
	unsigned int ref_count;
	pthread_mutex_t mutex;
	unsigned int size;
	unsigned int netpkt_offset, trapkt_offset, payload_offset;
	uint8_t *data;
};

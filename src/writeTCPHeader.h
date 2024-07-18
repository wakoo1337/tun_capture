void writeTCPHeader(uint8_t *data, unsigned int length, struct TCPHeaderData *hdr, uint8_t *pseudo, unsigned int pseudo_len, pthread_mutex_t *unlock_mutex);

unsigned int processTCPPacketImmediately(struct TCPConnection *connection, const struct IPPacketPayload *payload, const struct TCPHeaderData *header, unsigned int (*on_fin)(struct TCPConnection *));

struct UDPStackItem {
	struct RefcountBuffer *buffer;
	struct sockaddr dst;
	struct UDPStackItem *next;
};

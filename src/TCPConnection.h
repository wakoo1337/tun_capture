struct TCPConnection {
	struct SrcDstSockaddrs addrs; // src — адрес приложения, dst — адрес сайта
	int sock; // Сокет, связывающий с внешней сетью
	pthread_mutex_t mutex;
	const struct TCPState *state;
	const struct NetworkProtocolStrategy *strategy;
	struct CaptureContext *context;
	struct event *event;
};

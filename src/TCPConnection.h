struct TCPConnection {
	struct SrcDstSockaddrs addrs; // src — адрес приложения, dst — адрес сайта
	int sock; // Сокет, связывающий с внешней сетью
	pthread_mutex_t mutex;
	const struct TCPState *state;
	const struct NetworkProtocolStrategy *strategy;
	struct CaptureContext *context;
	struct event *event;
	struct TimeoutItem *timer;
	struct TCPSiteQueueItem *site_queue, **site_last;
	struct TCPAppQueueItem *app_queue, **app_last;
	uint32_t our_seq; // Наш номер последовательности, увеличивается с каждым следующим байтом
	uint32_t first_desired; // Номер первого байта, который мы желаем получить
	bool scaling_enabled; // Включено ли масштабирование окна?
	unsigned int our_scale, remote_scale; // Показатели масштабирования
};

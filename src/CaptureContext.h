struct CaptureContext {
	const struct CaptureSettings *settings; // Указатель на переданную пользователем структуру с настройками
	pthread_t *threads; // Потоки, их количество указывается в структуре настроек
	bool running;
	pthread_mutex_t rx_mutex, tx_mutex; // Мьютексы очередей
	pthread_cond_t rx_cond; // Условная переменная для извлечения пакетов из очереди приёма
	pthread_mutex_t event_mutex; // Общий мьютекс для операций с libevent
	struct PacketQueueItem *rx_begin, **rx_end; // Начало и конец очереди захваченных пакетов. captured_end указывает либо на next последнего пакета, либо на captured_begin, если очередь пуста
	struct PacketQueueItem *tx_begin, **tx_end; // Начало и конец очереди подлежащих отправке пакетов
	struct event_base *event_base;
	struct event *rx_event, *tx_event; // События приёма и передачи
	struct avl_table *ipv4_fragments; // Фрагменты пакетов IPv4
	struct event *timeout_event;
	heap_t *timeout_queue; // Очередь с приоритетом для таймаутов
	pthread_mutex_t timeout_mutex; // Мьютекс для таймеров
	pthread_mutex_t udp_mutex; // Мьютекс для контроля над привязками UDP
	struct avl_table *udp_bindings; // Таблица с привязками UDP
	pthread_mutex_t tcp_mutex; // Мьютекс для контроля над соединениями TCP
	struct avl_table *tcp_connections;
	uint16_t ipv4_id;
	uint32_t ipv6_id;
};

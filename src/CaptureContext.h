struct CaptureContext {
	const struct CaptureSettings *settings; // Указатель на переданную пользователем структуру с настройками
	pthread_t *threads; // Потоки, их количество указывается в структуре настроек
	pthread_mutex_t queue_mutex; // Мьютекс извлечения пакетов из стека
	pthread_cond_t queue_cond; // Условная переменная для извлечения пакетов из стека
	struct PacketQueueItem *captured_begin, **captured_end; // Начало и конец очереди захваченных пакетов. captured_end указывает либо на next последнего пакета, либо на captured_begin, если очередь пуста
	struct PacketQueueItem *send_stack; // Стек подлежащих отправке пакетов
	struct event_base *event_base;
	struct event *iface_event;
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

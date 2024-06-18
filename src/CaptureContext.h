struct CaptureContext {
	const struct CaptureSettings *settings; // Указатель на переданную пользователем структуру с настройками
	pthread_t *threads; // Потоки, их количество указывается в структуре настроек
	pthread_mutex_t queue_mutex; // Мьютекс извлечения пакетов из стека
	pthread_cond_t queue_cond; // Условная переменная для извлечения пакетов из стека
	struct PacketQueueItem *captured_stack; // Стек захваченных пакетов
	struct event_base *event_base;
	struct event *iface_event;
	struct avl_table *ipv4_fragments; // Фрагменты пакетов IPv4
	pq_t ipv4_fragq; // Очередь с приоритетом фрагментов IPv4, нужна для удаления просроченных
	pthread_mutex_t udp_mutex; // Мьютекс для контроля над привязками UDP
	struct avl_table *udp_bindings; // Таблица с привязками UDP
};

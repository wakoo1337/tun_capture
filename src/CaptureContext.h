struct CaptureContext {
	struct CaptureSettings *settings; // Указатель на переданную пользователем структуру с настройками
	pthread_t *threads; // Потоки, их количество указывается в структуре
	pthread_mutex_t queue_mutex; // Мьютекс извлечения пакетов из стека
	pthread_cond_t queue_cond; // Условная переменная для извлечения пакетов из стека
	struct PacketQueueItem *captured_stack; // Стек захваченных пакетов
	struct event_base *event_base;
	struct event *iface_event;
};

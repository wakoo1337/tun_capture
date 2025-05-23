struct TCPConnection {
	struct SrcDstSockaddrs addrs; // src — адрес приложения, dst — адрес сайта
	int sock; // Сокет, связывающий с внешней сетью
	pthread_mutex_t mutex;
	sem_t semaphore; // Семафор нужен, чтобы убедиться, что при уничтожении соединения его никто не держит
	const struct TCPState *state;
	const struct NetworkProtocolStrategy *strategy;
	struct CaptureContext *context;
	struct event *read_event, *write_event;
	bool read_alive, write_alive;
	unsigned int max_pktdata; // Максимальный размер данных в отправляемом пакете
	struct avl_table *site_prequeue; // Предочередь пакетов, поступающих на сайт
	struct avl_table *app_prequeue; // Предочередь пакетов, поступающих на приложение. Нужна, так как пакеты создаются многопоточно, и более ранний пакет может быть обработан позже
	struct TCPSiteQueueItem *site_queue, **site_last;
	struct TCPAppQueueItem *app_queue, **app_last;
	unsigned int site_scheduled, app_scheduled; // Сколько данных запланировано на отправку в каждую сторону
	uint32_t seq_next; // Номер следующего байта
	uint32_t seq_first; // SEQ пакета, который нужно извлечь из предочереди
	uint32_t first_desired; // Номер первого байта, который мы желаем получить, от него считается наше окно
	uint32_t latest_ack; // Последний ACK от приложения, от него считается окно приложения
	unsigned int app_window; // Размер окна приложения, с учётом масштабирования
	bool scaling_enabled; // Включено ли масштабирование окна?
	unsigned int our_scale, remote_scale; // Показатели масштабирования
	bool should_send_fin; // Нужно ли отправить FIN
	uint32_t fin_seq; // Номер последовательности, с которым будет отправлен FIN
	struct TimeoutItem *timewait_item; // Элемент таймаута для TIME_WAIT
};

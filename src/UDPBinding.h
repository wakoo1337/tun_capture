struct UDPBinding {
	struct sockaddr internal_addr; // Внутренний адрес
	int sock; // Сокет, связывающий с внешней сетью
	struct event *read_event, *write_event; // Событие, ожидаемое на сокете
	struct UDPStackItem *stack; // Стек отправки
	pthread_mutex_t mutex;
	sem_t semaphore;
	struct CaptureContext *context;
	bool persistent, read_alive, write_alive;
	struct TimeoutItem *timeout;
};

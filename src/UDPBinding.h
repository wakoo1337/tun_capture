struct UDPBinding {
	struct sockaddr internal_addr; // Внутренний адрес
	int sock; // Сокет, связывающий с внешней сетью
	struct event *sock_event; // Событие, ожидаемое на сокете
	struct UDPQueueItem *queue; // Очередь отправки
	pthread_mutex_t mutex; // Мьютекс для управления событиями и контроля над очередью
	struct CaptureContext *context;
};

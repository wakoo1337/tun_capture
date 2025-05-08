struct TimeoutItem {
	struct timeval expiration; // Время истечения таймаута
	bool (*callback)(struct CaptureContext *, struct TimeoutItem *); // Функция, вызываемая по таймауту
	void *arg; // Её аргумент, зависит от конкретной функции
	unsigned int index; // Индекс в куче
};

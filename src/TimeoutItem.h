struct TimeoutItem {
	struct timeval expiration; // Время истечения таймаута
	void (*callback)(void *); // Функция, вызываемая по таймауту
	void *arg; // Её аргумент, зависит от конкретной функции
	bool is_del; // Удалять ли элемент сразу
};

struct UDPStackItem {
	uint8_t *send_me; // Указатель, откуда отправлять
	unsigned int size; // Количество данных для отправки
	void *free_me; // Что освобождать после отправки. Send_me указывает внутрь IP-пакета, тогда как free_me указывает на его начало
	struct sockaddr dst;
	struct UDPStackItem *next;
};

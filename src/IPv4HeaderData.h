struct IPv4HeaderData {
	unsigned int len; // Длина заголовка
	uint8_t tos; // Тип обслуживания, нафиг не нужен
	uint16_t total_length; // Длина всей датаграммы
	uint16_t identification; // Идентификатор
	bool df; // Не фрагментировать
	bool mf; // Ещё фрагменты
	unsigned int fragment_offset; // Смещение фрагмента
	uint8_t ttl; // Время жизни
	uint8_t protocol; // Протокол
	uint16_t header_cs; // Контрольная сумма заголовка
	struct in_addr src; // Адрес источника
	struct in_addr dst; // Адрес назначения
};

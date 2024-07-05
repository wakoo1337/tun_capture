struct TCPHeaderData {
	uint16_t src_port; // Порт источника
	uint16_t dst_port; // Порт назначения
	uint32_t seq_num; // Номер последовательности
	uint32_t ack_num; // Номер подтверждения
	unsigned int data_offset; // Смещение данных
	bool urg; // Флаг URG
	bool ack; // Флаг ACK
	bool psh; // Флаг PSH
	bool rst; // Флаг RST
	bool syn; // Флаг SYN
	bool fin; // Флаг FIN
	uint16_t raw_window; // Размер окна в пакете, без учёта масштабирования
	uint16_t checksum; // Контрольная сумма
	uint16_t urgent_ptr; // Указатель срочности
	bool mss_present;
	uint16_t mss_value;
	bool winscale_present;
	uint8_t winscale_value;
};

struct IPFragmentMetadata {
	unsigned int header_size; // Размер заголовка
	unsigned int data_size; // Размер данных в пакете
	unsigned int fragment_offset; // Смещение фрагмента, в байтах
	uint8_t *buffer;
};

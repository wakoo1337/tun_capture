struct IPv4Fragments {
	unsigned int id; // Идентификатор фрагментов
	struct in_addr src; // Отправитель
	struct IPv4Fragment *first; // Указатель на первый фрагмент
	struct TimeoutItem *timeout;
};

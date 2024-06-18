struct IPv4Fragments {
	unsigned int id; // Идентификатор фрагментов
	struct in_addr src; // Отправитель
	struct timespec expiration; // Время, когда все фрагменты будут удалены
	struct IPv4Fragment *first; // Указатель на первый фрагмент
	bool del_this; // Удалять ли этот фрагмент, когда он на вершине очереди -- очередь с приоритетом не поддерживает удаление не с начала
};

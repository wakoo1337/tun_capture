struct CaptureSettings {
	uint16_t mtu; // MTU интерфейса
	uint8_t ttl; // TTL генерируемых пакетов
	unsigned int threads_count; // Число потоков
	struct ForwardingMappingIPv4 *v4_tcpmap, *v4_udpmap;
	struct ForwardingMappingIPv6 *v6_tcpmap, *v6_udpmap;
	unsigned int v4_tcpcount, v4_udpcount;
	unsigned int v6_tcpcount, v6_udpcount;
	int (*fd_getter)(void *); // Функция, возвращающая дескриптор сетевого интерфейса
	ssize_t (*read_function)(void *, size_t, void *); // Функция, которая считывает пакеты из интерфейса
	ssize_t (*write_function)(void *, size_t, void *); // Функция, которая записывает пакеты в интерфейс. В случае veth, для этого нужно использовать не write(), а sendto()
	void *user; // Указатель на пользовательские данные, которые передаются в callback-функции
};

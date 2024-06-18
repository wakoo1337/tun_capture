struct CaptureSettings {
	int fd; // Файловый дескриптор интерфейса
	uint16_t tun_mtu; // MTU интерфейса
	unsigned int threads_count; // Число потоков
	struct ForwardingMappingIPv4 v4_tcpmap, v4_udpmap;
	struct ForwardingMappingIPv6 v6_tcpmap, v6_udpmap;
	unsigned int v4_tcpcount, v4_udpcount;
	unsigned int v6_tcpcount, v6_udpcount;
	void (*packet_callback)(unsigned int count, void *bytes, bool direction, void *user); // Функция, которой передаются захваченные и сгенерированные пакеты
	void *user; // Указатель на пользовательские данные, которые передаются в callback-функцию
};

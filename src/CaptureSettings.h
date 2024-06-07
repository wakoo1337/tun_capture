struct CaptureSettings {
	int fd; // Файловый дескриптор интерфейса
	uint16_t tun_mtu; // MTU интерфейса
	unsigned int threads_count; // Число потоков
	struct in_addr addr_v4; // Сетевой адрес IPv4
	struct in6_addr addr_v6; // Сетевой адрес IPv6
	unsigned int tcps_count, udps_count; // Количество портов TCP и UDP
	uint16_t *tcp_ports; // Порты TCP, подлежащие пробрасыванию
	uint16_t *udp_ports; // Порты UDP, подлежащие пробрасыванию
	void (*packet_callback)(unsigned int count, void *bytes, bool direction, void *user); // Функция, которой передаются захваченные и сгенерированные пакеты
	void *user; // Указатель на пользовательские данные, которые передаются в callback-функцию
	void *context; // Контекст захвата трафика, используется внутренними функциями
};

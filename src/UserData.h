struct UserData {
	int fd; // Дескриптор файлового интерфейса
	FILE *cap_file; // Файл, куда сохраняется захваченный трафик
	struct sockaddr sa; // struct sockaddr, указывающая на удалённый интерфейс
};

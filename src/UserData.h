struct UserData {
	int fd; // Дескриптор файлового интерфейса
	FILE *cap_file; // Файл, куда сохраняется захваченный трафик
	struct sockaddr_ll sa; // struct sockaddr_ll, указывающая на удалённый интерфейс
};

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include "CaptureSettings.h"
#include "openInterface.h"
#include "doCapture.h"

int main(int argc, char **argv) {
	struct CaptureSettings settings;
	if (argc < 2) {
		puts("Укажите номер интерфейса, соединённого с приложениями");
		return 1;
	};
	int iface;
	sscanf(argv[1], "%i", &iface);
	settings.fd = openInterface(iface);
	if (0 == settings.fd) return 1;
	doCapture(&settings);
	return 0;
};

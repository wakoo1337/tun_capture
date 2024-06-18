#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <time.h>
#include "ForwardingMappingIPv4.h"
#include "ForwardingMappingIPv6.h"
#include "CaptureSettings.h"
#include "openInterface.h"
#include "doCapture.h"
#include "saveCallback.h"
#include "set32Bit.h"
#include "set16Bit.h"

int main(int argc, char **argv) {
	struct CaptureSettings settings;
	if (argc < 2) {
		puts("Укажите номер интерфейса, соединённого с приложениями");
		return 1;
	};
	int iface;
	sscanf(argv[1], "%i", &iface);
	settings.fd = openInterface(iface);
	if (-1 == settings.fd) return 1;
	settings.tun_mtu = 1500;
	settings.threads_count = 8;
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	struct tm broken_time;
	localtime_r(&ts.tv_sec, &broken_time);
	char path_buffer[100];
	strftime(path_buffer, 100, "%F_%H-%M-%S.pcap", &broken_time);
	settings.user = fopen(path_buffer, "wb");
	unsigned char header[24];
	set32Bit(&header[0], 0xa1b2c3d4);
	set16Bit(&header[4], 2);
	set16Bit(&header[6], 4);
	set32Bit(&header[8], 0);
	set32Bit(&header[12], 0);
	set32Bit(&header[16], 65535);
	set32Bit(&header[20], 101);
	fwrite(header, sizeof(char), 24, (FILE *) settings.user);
	settings.packet_callback = &saveCallback;
	doCapture(&settings);
	return 0;
};

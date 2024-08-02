#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <time.h>
#include "ForwardingMappingIPv4.h"
#include "ForwardingMappingIPv6.h"
#include "UserData.h"
#include "CaptureSettings.h"
#include "openInterface.h"
#include "doCapture.h"
#include "set32Bit.h"
#include "set16Bit.h"
#include "getFdFromUser.h"
#include "readPacket.h"
#include "writePacket.h"

int main(int argc, char **argv) {
	struct CaptureSettings settings;
	if (argc < 2) {
		puts("Укажите номер интерфейса, соединённого с приложениями");
		return 1;
	};
	struct UserData user;
	int iface;
	sscanf(argv[1], "%i", &iface);
	user.fd = openInterface(iface);
	if (-1 == user.fd) return 1;
	settings.mtu = 1500;
	settings.ttl = 100;
	settings.threads_count = 8;
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	struct tm broken_time;
	localtime_r(&ts.tv_sec, &broken_time);
	char path_buffer[100];
	strftime(path_buffer, 100, "%F_%H-%M-%S.pcap", &broken_time);
	user.cap_file = fopen(path_buffer, "wb");
	unsigned char header[24];
	set32Bit(&header[0], 0xa1b2c3d4);
	set16Bit(&header[4], 2);
	set16Bit(&header[6], 4);
	set32Bit(&header[8], 0);
	set32Bit(&header[12], 0);
	set32Bit(&header[16], 65535);
	set32Bit(&header[20], 101);
	fwrite(header, sizeof(char), 24, user.cap_file);
	user.sa = (struct sockaddr) {0};
	((struct sockaddr_ll *) &user.sa)->sll_family = AF_PACKET;
	((struct sockaddr_ll *) &user.sa)->sll_protocol = htons(ETH_P_IP);
	((struct sockaddr_ll *) &user.sa)->sll_ifindex = iface;
	((struct sockaddr_ll *) &user.sa)->sll_halen = 6;
	((struct sockaddr_ll *) &user.sa)->sll_addr[0] = 0x1E;
	((struct sockaddr_ll *) &user.sa)->sll_addr[1] = 0x3D;
	((struct sockaddr_ll *) &user.sa)->sll_addr[2] = 0x27;
	((struct sockaddr_ll *) &user.sa)->sll_addr[3] = 0xAF;
	((struct sockaddr_ll *) &user.sa)->sll_addr[4] = 0x3A;
	((struct sockaddr_ll *) &user.sa)->sll_addr[5] = 0x7C;
	settings.user = &user;
	settings.fd_getter = &getFdFromUser;
	settings.read_function = &readPacket;
	settings.write_function = &writePacket;
	doCapture(&settings);
	fclose(user.cap_file);
	return 0;
};

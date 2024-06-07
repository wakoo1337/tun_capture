#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "openInterface.h"
int openInterface(int card) {
	int sock;
	sock = socket(AF_PACKET, SOCK_DGRAM, 0);
	if (-1 == sock) {
		perror("socket");
		return -1;
	};
	struct sockaddr_ll sll = {0};
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = card;
	sll.sll_protocol = htons(ETH_P_ALL);
	if (-1 == bind(sock, (struct sockaddr *) &sll, sizeof(struct sockaddr_ll))) {
		close(sock);
		perror("bind");
		return -1;
	};
	return sock;
};

#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "setIPv4SockaddrPort.h"
void setIPv4SockaddrPort(struct sockaddr *sa, uint16_t port) {
	((struct sockaddr_in *) sa)->sin_port = htons(port);
};

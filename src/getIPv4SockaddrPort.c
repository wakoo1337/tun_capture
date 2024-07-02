#include <netinet/in.h>
#include <arpa/inet.h>

#include "getIPv4SockaddrPort.h"
uint16_t getIPv4SockaddrPort(const struct sockaddr *sa) {
	return ntohs(((struct sockaddr_in *) sa)->sin_port);
};

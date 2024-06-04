#include <net/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "openTunInterface.h"
int openTunInterface(const char *name) {
	struct ifreq ifr;
	int fd = open("/dev/net/tun", O_RDWR);
	if (-1 == fd) return -1;
	memset(&ifr, 0, sizeof ifr);
	strncpy(ifr.ifr_name, name, IFNAMSIZ);
	ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
	if (-1 == ioctl(fd, TUNSETIFF, &ifr)) {
		close(fd);
		return -1;
	};
	return fd;
};

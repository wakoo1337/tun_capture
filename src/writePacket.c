#include <sys/socket.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <errno.h>
#include "UserData.h"
#include "savePacket.h"

#include "writePacket.h"
ssize_t writePacket(void *bytes, size_t count, void *user) {
	savePacket(((struct UserData *) user)->cap_file, bytes, count);
	ssize_t result = sendto(((struct UserData *) user)->fd, bytes, count, 0, &((struct UserData *) user)->sa, sizeof(struct sockaddr_ll));
	return result;
};

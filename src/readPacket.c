#include <sys/socket.h>
#include <stdio.h>
#include "UserData.h"
#include "savePacket.h"

#include "readPacket.h"
ssize_t readPacket(void *bytes, size_t count, void *user) {
	ssize_t result;
	struct sockaddr sa;
	socklen_t sl = sizeof(struct sockaddr);
	result = recvfrom(((struct UserData *) user)->fd, bytes, count, 0, &sa, &sl);
	if (result == -1) {
		return -1;
	} else {
		savePacket(((struct UserData *) user)->cap_file, bytes, result);
		return result;
	};
};

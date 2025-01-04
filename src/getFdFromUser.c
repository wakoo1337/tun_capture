#include <sys/socket.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include "UserData.h"

#include "getFdFromUser.h"
int getFdFromUser(void *user) {
	return ((struct UserData *) user)->fd;
};

#include <sys/socket.h>
#include <stdio.h>
#include "UserData.h"

#include "getFdFromUser.h"
int getFdFromUser(void *user) {
	return ((struct UserData *) user)->fd;
};

#include "openTunInterface.h"

int main(int argc, char **argv) {
	int fd;
	fd = openTunInterface("capture_tun");
	return 0;
}; 

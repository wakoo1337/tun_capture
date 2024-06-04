LINUX_CFLAGS := 
LIBEVENT_LIBS := -levent
CC := cc
CFLAGS := -O0 -g -Wall
.PHONY: all
all: bin/tun_capture
bin/tun_capture: bin/tun_capture.o bin/openTunInterface.o
bin/tun_capture.o: src/tun_capture.c
	$(CC) $(CFLAGS) -c -o bin/tun_capture.o src/tun_capture.c
bin/openTunInterface.o: src/openTunInterface.c src/openTunInterface.h
	$(CC) $(CFLAGS) $(LINUX_CFLAGS) -c -o bin/openTunInterface.o src/openTunInterface.c

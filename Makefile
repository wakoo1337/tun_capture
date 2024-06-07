LIBEVENT_LIBS := -levent
LIBEVENT_CFLAGS := 
CC := cc
CFLAGS := -O0 -g -Wall -std=gnu11
.PHONY: all
all: bin/tun_capture
bin/tun_capture: bin/tun_capture.o bin/openInterface.o bin/doCapture.o bin/tunCallback.o
	$(CC) $(CFLAGS) -o bin/tun_capture bin/tun_capture.o bin/openInterface.o bin/doCapture.o bin/tunCallback.o $(LIBEVENT_LIBS)
bin/tun_capture.o: src/tun_capture.c src/CaptureSettings.h src/openInterface.h src/doCapture.h
	$(CC) $(CFLAGS) -c -o bin/tun_capture.o src/tun_capture.c
bin/openInterface.o: src/openInterface.c src/openInterface.h
	$(CC) $(CFLAGS) -c -o bin/openInterface.o src/openInterface.c
bin/doCapture.o: src/doCapture.c src/doCapture.h src/CaptureContext.h src/CaptureSettings.h src/tunCallback.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/doCapture.o src/doCapture.c
bin/tunCallback.o: src/tunCallback.c src/tunCallback.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tunCallback.o src/tunCallback.c
clean:
	rm -f bin/*

LIBEVENT_LIBS := -levent
LIBEVENT_CFLAGS := 
CC := cc
CFLAGS := -O0 -g -Wall -std=gnu11
.PHONY: all
all: bin/tun_capture
bin/tun_capture: bin/tun_capture.o bin/openInterface.o bin/doCapture.o bin/tunCallback.o bin/set16Bit.o bin/set32Bit.o bin/saveCallback.o bin/dequeuePacket.o bin/threadWorker.o
	$(CC) $(CFLAGS) -o bin/tun_capture bin/tun_capture.o bin/openInterface.o bin/doCapture.o bin/tunCallback.o bin/set16Bit.o bin/set32Bit.o bin/saveCallback.o bin/dequeuePacket.o bin/threadWorker.o $(LIBEVENT_LIBS)
bin/tun_capture.o: src/tun_capture.c src/CaptureSettings.h src/openInterface.h src/doCapture.h src/saveCallback.h src/set32Bit.h src/set16Bit.h
	$(CC) $(CFLAGS) -c -o bin/tun_capture.o src/tun_capture.c
bin/openInterface.o: src/openInterface.c src/openInterface.h
	$(CC) $(CFLAGS) -c -o bin/openInterface.o src/openInterface.c
bin/doCapture.o: src/doCapture.c src/doCapture.h src/CaptureContext.h src/CaptureSettings.h src/tunCallback.h src/threadWorker.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/doCapture.o src/doCapture.c
bin/tunCallback.o: src/tunCallback.c src/tunCallback.h src/PacketQueueItem.h src/CaptureContext.h src/CaptureSettings.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tunCallback.o src/tunCallback.c
bin/set16Bit.o: src/set16Bit.c src/set16Bit.h
	$(CC) $(CFLAGS) -c -o bin/set16Bit.o src/set16Bit.c
bin/set32Bit.o: src/set32Bit.c src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/set32Bit.o src/set32Bit.c
bin/saveCallback.o: src/saveCallback.c src/saveCallback.h src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/saveCallback.o src/saveCallback.c
bin/dequeuePacket.o: src/dequeuePacket.c src/dequeuePacket.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/dequeuePacket.o src/dequeuePacket.c
bin/threadWorker.o: src/threadWorker.c src/threadWorker.h src/CaptureContext.h src/PacketQueueItem.h src/dequeuePacket.h
	$(CC) $(CFLAGS) -c -o bin/threadWorker.o src/threadWorker.c
clean:
	rm -f bin/*

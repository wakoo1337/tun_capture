LIBEVENT_LIBS := -levent
LIBEVENT_CFLAGS := 
AR := ar
CC := cc
CFLAGS := -O0 -g -Wall -std=gnu11
.PHONY: all
all: bin/tun_capture
bin/tun_capture: bin/tun_capture.o bin/openInterface.o bin/doCapture.o bin/tunCallback.o bin/set16Bit.o bin/set32Bit.o bin/savePacket.o bin/dequeuePacket.o bin/threadWorker.o bin/compareIPv4FragmentsIdsSources.o bin/compareIPv4Queue.o bin/processIPv4Packet.o bin/compareTimespec.o bin/get16Bit.o bin/get32Bit.o bin/parseIPv4Header.o bin/addShortWithCarry.o bin/udpCallback.o bin/compareUDPBindings.o bin/processUDPPacket.o bin/setIPv4SockaddrPort.o bin/findUDPBinding.o bin/packetsProcessor.o bin/udpGenerator.o bin/computeIPv4FragmentsCount.o bin/ipv4_strategy.o bin/fillIPv4FragmentMetadatas.o bin/createIPv4PseudoHdr.o bin/writeIPv4Headers.o bin/getIPv4SockaddrPort.o bin/readPacket.o bin/writePacket.o bin/getFdFromUser.o bin/checksum.a bin/avl.o bin/PQ.o
	$(CC) $(CFLAGS) -o bin/tun_capture bin/tun_capture.o bin/openInterface.o bin/doCapture.o bin/tunCallback.o bin/set16Bit.o bin/set32Bit.o bin/savePacket.o bin/dequeuePacket.o bin/threadWorker.o bin/compareIPv4FragmentsIdsSources.o bin/compareIPv4Queue.o bin/compareTimespec.o bin/processIPv4Packet.o bin/get16Bit.o bin/get32Bit.o bin/parseIPv4Header.o bin/addShortWithCarry.o bin/udpCallback.o bin/compareUDPBindings.o bin/processUDPPacket.o bin/setIPv4SockaddrPort.o bin/findUDPBinding.o bin/packetsProcessor.o bin/udpGenerator.o bin/computeIPv4FragmentsCount.o bin/ipv4_strategy.o bin/fillIPv4FragmentMetadatas.o bin/createIPv4PseudoHdr.o bin/writeIPv4Headers.o bin/getIPv4SockaddrPort.o bin/readPacket.o bin/writePacket.o bin/getFdFromUser.o bin/checksum.a bin/avl.o bin/PQ.o $(LIBEVENT_LIBS)
bin/tun_capture.o: src/tun_capture.c src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/UserData.h src/CaptureSettings.h src/openInterface.h src/doCapture.h src/set32Bit.h src/set16Bit.h src/getFdFromUser.h src/readPacket.h src/writePacket.h
	$(CC) $(CFLAGS) -c -o bin/tun_capture.o src/tun_capture.c
bin/openInterface.o: src/openInterface.c src/openInterface.h
	$(CC) $(CFLAGS) -c -o bin/openInterface.o src/openInterface.c
bin/doCapture.o: src/doCapture.c src/doCapture.h src/contrib/avl.h src/contrib/C-Collections/pqlib/PQ.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureContext.h src/CaptureSettings.h src/tunCallback.h src/threadWorker.h src/compareIPv4FragmentsIdsSources.h src/compareIPv4Queue.h src/compareUDPBindings.h src/UDPBinding.h src/udpCallback.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/doCapture.o src/doCapture.c
bin/tunCallback.o: src/tunCallback.c src/tunCallback.h src/contrib/C-Collections/pqlib/PQ.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureSettings.h src/CaptureContext.h src/PacketQueueItem.h src/packetsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tunCallback.o src/tunCallback.c
bin/set16Bit.o: src/set16Bit.c src/set16Bit.h
	$(CC) $(CFLAGS) -c -o bin/set16Bit.o src/set16Bit.c
bin/set32Bit.o: src/set32Bit.c src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/set32Bit.o src/set32Bit.c
bin/savePacket.o: src/savePacket.c src/savePacket.h src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/savePacket.o src/savePacket.c
bin/dequeuePacket.o: src/dequeuePacket.c src/dequeuePacket.h src/contrib/C-Collections/pqlib/PQ.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/dequeuePacket.o src/dequeuePacket.c
bin/threadWorker.o: src/threadWorker.c src/threadWorker.h src/contrib/C-Collections/pqlib/PQ.h src/CaptureContext.h src/PacketQueueItem.h src/dequeuePacket.h
	$(CC) $(CFLAGS) -c -o bin/threadWorker.o src/threadWorker.c
bin/compareIPv4FragmentsIdsSources.o: src/compareIPv4FragmentsIdsSources.c src/compareIPv4FragmentsIdsSources.h src/IPv4Fragments.h
	$(CC) $(CFLAGS) -c -o bin/compareIPv4FragmentsIdsSources.o src/compareIPv4FragmentsIdsSources.c
bin/compareIPv4Queue.o: src/compareIPv4Queue.c src/compareIPv4Queue.h src/contrib/C-Collections/pqlib/PQ.h src/IPv4Fragments.h src/compareTimespec.h
	$(CC) $(CFLAGS) -c -o bin/compareIPv4Queue.o src/compareIPv4Queue.c
bin/compareTimespec.o: src/compareTimespec.c src/compareTimespec.h
	$(CC) $(CFLAGS) -c -o bin/compareTimespec.o src/compareTimespec.c
bin/checksum.a: bin/computeChecksum.o bin/initChecksum.o bin/getChecksum.o
	$(AR) rcs bin/checksum.a bin/computeChecksum.o bin/initChecksum.o bin/getChecksum.o
bin/computeChecksum.o: src/computeChecksum.c src/computeChecksum.h src/ChecksumContext.h src/addShortWithCarry.h
	$(CC) $(CFLAGS) -c -o bin/computeChecksum.o src/computeChecksum.c
bin/initChecksum.o: src/initChecksum.c src/initChecksum.h src/ChecksumContext.h
	$(CC) $(CFLAGS) -c -o bin/initChecksum.o src/initChecksum.c
bin/getChecksum.o: src/getChecksum.c src/getChecksum.h src/ChecksumContext.h
	$(CC) $(CFLAGS) -c -o bin/getChecksum.o src/getChecksum.c
bin/processIPv4Packet.o: src/processIPv4Packet.c src/processIPv4Packet.h src/contrib/C-Collections/pqlib/PQ.h src/CaptureContext.h src/IPv4HeaderData.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/parseIPv4Header.h src/processUDPPacket.h src/setIPv4SockaddrPort.h src/ipv4_strategy.h
	$(CC) $(CFLAGS) -c -o bin/processIPv4Packet.o src/processIPv4Packet.c
bin/parseIPv4Header.o: src/parseIPv4Header.c src/parseIPv4Header.h src/IPv4HeaderData.h src/ChecksumContext.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/get16Bit.h src/get32Bit.h
	$(CC) $(CFLAGS) -c -o bin/parseIPv4Header.o src/parseIPv4Header.c
bin/get16Bit.o: src/get16Bit.c src/get16Bit.h
	$(CC) $(CFLAGS) -c -o bin/get16Bit.o src/get16Bit.c
bin/get32Bit.o: src/get32Bit.c src/get32Bit.h
	$(CC) $(CFLAGS) -c -o bin/get32Bit.o src/get32Bit.c
bin/addShortWithCarry.o: src/addShortWithCarry.c src/addShortWithCarry.h
	$(CC) $(CFLAGS) -c -o bin/addShortWithCarry.o src/addShortWithCarry.c
bin/udpCallback.o: src/udpCallback.c src/udpCallback.h src/contrib/C-Collections/pqlib/PQ.h src/UDPQueueItem.h src/UDPBinding.h src/UDPParameters.h src/CaptureContext.h src/PacketQueueItem.h src/HEADERS_RESERVE.h src/MAX_UDP_PAYLOAD.h src/udpGenerator.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/udpCallback.o src/udpCallback.c
bin/compareUDPBindings.o: src/compareUDPBindings.c src/compareUDPBindings.h src/UDPBinding.h
	$(CC) $(CFLAGS) -c -o bin/compareUDPBindings.o src/compareUDPBindings.c
bin/processUDPPacket.o: src/processUDPPacket.c src/processUDPPacket.h src/contrib/C-Collections/pqlib/PQ.h src/CaptureContext.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/UDPHeaderData.h src/UDPQueueItem.h src/ChecksumContext.h src/UDPBinding.h src/get16Bit.h src/set16Bit.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/findUDPBinding.h src/udpCallback.h
	$(CC) $(CFLAGS) -c -o bin/processUDPPacket.o src/processUDPPacket.c
bin/setIPv4SockaddrPort.o: src/setIPv4SockaddrPort.c src/setIPv4SockaddrPort.h
	$(CC) $(CFLAGS) -c -o bin/setIPv4SockaddrPort.o src/setIPv4SockaddrPort.c
bin/findUDPBinding.o: src/findUDPBinding.c src/findUDPBinding.h src/contrib/avl.h src/contrib/C-Collections/pqlib/PQ.h src/CaptureContext.h src/UDPBinding.h src/udpCallback.h
	$(CC) $(CFLAGS) -c -o bin/findUDPBinding.o src/findUDPBinding.c
bin/packetsProcessor.o: src/packetsProcessor.c src/packetsProcessor.h src/contrib/C-Collections/pqlib/PQ.h src/CaptureContext.h src/processIPv4Packet.h
	$(CC) $(CFLAGS) -c -o bin/packetsProcessor.o src/packetsProcessor.c
bin/udpGenerator.o: src/udpGenerator.c src/udpGenerator.h src/contrib/C-Collections/pqlib/PQ.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureSettings.h src/CaptureContext.h src/UDPParameters.h src/UDPBinding.h src/ChecksumContext.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/PacketQueueItem.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/tunCallback.h src/HEADERS_RESERVE.h src/ipv4_strategy.h src/ipv6_strategy.h
	$(CC) $(CFLAGS) -c -o bin/udpGenerator.o src/udpGenerator.c
bin/computeIPv4FragmentsCount.o: src/computeIPv4FragmentsCount.c src/computeIPv4FragmentsCount.h
	$(CC) $(CFLAGS) -c -o bin/computeIPv4FragmentsCount.o src/computeIPv4FragmentsCount.c
bin/ipv4_strategy.o: src/ipv4_strategy.c src/ipv4_strategy.h src/contrib/C-Collections/pqlib/PQ.h src/CaptureContext.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/setIPv4SockaddrPort.h src/getIPv4SockaddrPort.h src/computeIPv4FragmentsCount.h src/fillIPv4FragmentMetadatas.h src/createIPv4PseudoHdr.h src/writeIPv4Headers.h
	$(CC) $(CFLAGS) -c -o bin/ipv4_strategy.o src/ipv4_strategy.c
bin/fillIPv4FragmentMetadatas.o: src/fillIPv4FragmentMetadatas.c src/fillIPv4FragmentMetadatas.h src/IPFragmentMetadata.h
	$(CC) $(CFLAGS) -c -o bin/fillIPv4FragmentMetadatas.o src/fillIPv4FragmentMetadatas.c
bin/createIPv4PseudoHdr.o: src/createIPv4PseudoHdr.c src/createIPv4PseudoHdr.h
	$(CC) $(CFLAGS) -c -o bin/createIPv4PseudoHdr.o src/createIPv4PseudoHdr.c
bin/writeIPv4Headers.o: src/writeIPv4Headers.c src/writeIPv4Headers.h src/contrib/C-Collections/pqlib/PQ.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureSettings.h src/CaptureContext.h src/IPFragmentMetadata.h src/ChecksumContext.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h
	$(CC) $(CFLAGS) -c -o bin/writeIPv4Headers.o src/writeIPv4Headers.c
bin/getIPv4SockaddrPort.o: src/getIPv4SockaddrPort.c src/getIPv4SockaddrPort.h
	$(CC) $(CFLAGS) -c -o bin/getIPv4SockaddrPort.o src/getIPv4SockaddrPort.c
bin/readPacket.o: src/readPacket.c src/readPacket.h src/UserData.h src/savePacket.h
	$(CC) $(CFLAGS) -c -o bin/readPacket.o src/readPacket.c
bin/writePacket.o: src/writePacket.c src/writePacket.h src/UserData.h src/savePacket.h
	$(CC) $(CFLAGS) -c -o bin/writePacket.o src/writePacket.c
bin/getFdFromUser.o: src/getFdFromUser.c src/getFdFromUser.h src/UserData.h
	$(CC) $(CFLAGS) -c -o bin/getFdFromUser.o src/getFdFromUser.c
bin/avl.o: src/contrib/avl.c src/contrib/avl.h
	$(CC) $(CFLAGS) -c -o bin/avl.o src/contrib/avl.c
bin/PQ.o: src/contrib/C-Collections/pqlib/PQ.c src/contrib/C-Collections/pqlib/PQ.h
	$(CC) $(CFLAGS) -c -o bin/PQ.o src/contrib/C-Collections/pqlib/PQ.c
clean:
	rm -f bin/*

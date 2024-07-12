LIBEVENT_LIBS := -levent
LIBEVENT_CFLAGS := 
AR := ar
CC := cc
CFLAGS := -O0 -g -Wall -std=gnu11
.PHONY: all
all: bin/tun_capture
bin/tun_capture: bin/tun_capture.o bin/openInterface.o bin/doCapture.o bin/tunCallback.o bin/set16Bit.o bin/set32Bit.o bin/savePacket.o bin/dequeuePacket.o bin/threadWorker.o bin/compareIPv4FragmentsIdsSources.o bin/compareTimeoutItems.o bin/processIPv4Packet.o bin/compareTimeval.o bin/get16Bit.o bin/get32Bit.o bin/parseIPv4Header.o bin/addShortWithCarry.o bin/udpCallback.o bin/compareUDPBindings.o bin/processUDPPacket.o bin/setIPv4SockaddrPort.o bin/findUDPBinding.o bin/packetsProcessor.o bin/udpGenerator.o bin/computeIPv4FragmentsCount.o bin/ipv4_strategy.o bin/fillIPv4FragmentMetadatas.o bin/createIPv4PseudoHdr.o bin/writeIPv4Headers.o bin/getIPv4SockaddrPort.o bin/readPacket.o bin/writePacket.o bin/getFdFromUser.o bin/addTimeval.o bin/subTimeval.o bin/getMonotonicTimeval.o bin/timerCallback.o bin/startTimer.o bin/parseTCPHeader.o bin/processTCPPacket.o bin/compareTCPConnections.o bin/tcpCallback.o bin/tcpstate_connwait.o bin/tcpConnwaitEventCallback.o bin/destroyTCPConnection.o bin/tcpConnwaitPacketsProcessor.o bin/writeTCPHeader.o bin/tcpSynackSendEventCallback.o bin/tcpSynackSendPacketsProcessor.o bin/tcpstate_synack_send.o bin/getSendWindowSize.o bin/computeIPv4MSS.o bin/computeIPv6MSS.o bin/sendSynReply.o bin/retry_delay.o bin/enqueueTimeout.o bin/sendTCPPacket.o bin/tcpRetransmissionTimerCallback.o bin/computeTCPDataOffset.o bin/checksum.a bin/avl.o bin/heap.o
	$(CC) $(CFLAGS) -o bin/tun_capture bin/tun_capture.o bin/openInterface.o bin/doCapture.o bin/tunCallback.o bin/set16Bit.o bin/set32Bit.o bin/savePacket.o bin/dequeuePacket.o bin/threadWorker.o bin/compareIPv4FragmentsIdsSources.o bin/compareTimeoutItems.o bin/compareTimeval.o bin/processIPv4Packet.o bin/get16Bit.o bin/get32Bit.o bin/parseIPv4Header.o bin/addShortWithCarry.o bin/udpCallback.o bin/compareUDPBindings.o bin/processUDPPacket.o bin/setIPv4SockaddrPort.o bin/findUDPBinding.o bin/packetsProcessor.o bin/udpGenerator.o bin/computeIPv4FragmentsCount.o bin/ipv4_strategy.o bin/fillIPv4FragmentMetadatas.o bin/createIPv4PseudoHdr.o bin/writeIPv4Headers.o bin/getIPv4SockaddrPort.o bin/readPacket.o bin/writePacket.o bin/getFdFromUser.o bin/addTimeval.o bin/subTimeval.o bin/getMonotonicTimeval.o bin/timerCallback.o bin/startTimer.o bin/parseTCPHeader.o bin/processTCPPacket.o bin/compareTCPConnections.o bin/tcpCallback.o bin/tcpstate_connwait.o bin/tcpConnwaitEventCallback.o bin/destroyTCPConnection.o bin/tcpConnwaitPacketsProcessor.o bin/writeTCPHeader.o bin/tcpSynackSendEventCallback.o bin/tcpSynackSendPacketsProcessor.o bin/tcpstate_synack_send.o bin/getSendWindowSize.o bin/computeIPv4MSS.o bin/computeIPv6MSS.o bin/sendSynReply.o bin/retry_delay.o bin/enqueueTimeout.o bin/sendTCPPacket.o bin/tcpRetransmissionTimerCallback.o bin/computeTCPDataOffset.o bin/checksum.a bin/avl.o bin/heap.o $(LIBEVENT_LIBS)
bin/tun_capture.o: src/tun_capture.c src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/UserData.h src/CaptureSettings.h src/openInterface.h src/doCapture.h src/set32Bit.h src/set16Bit.h src/getFdFromUser.h src/readPacket.h src/writePacket.h
	$(CC) $(CFLAGS) -c -o bin/tun_capture.o src/tun_capture.c
bin/openInterface.o: src/openInterface.c src/openInterface.h
	$(CC) $(CFLAGS) -c -o bin/openInterface.o src/openInterface.c
bin/doCapture.o: src/doCapture.c src/doCapture.h src/contrib/avl.h src/contrib/heap.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureContext.h src/CaptureSettings.h src/tunCallback.h src/threadWorker.h src/compareIPv4FragmentsIdsSources.h src/compareTimeoutItems.h src/compareUDPBindings.h src/UDPBinding.h src/udpCallback.h src/timerCallback.h src/compareTCPConnections.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/doCapture.o src/doCapture.c
bin/tunCallback.o: src/tunCallback.c src/tunCallback.h src/contrib/heap.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureSettings.h src/CaptureContext.h src/PacketQueueItem.h src/packetsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tunCallback.o src/tunCallback.c
bin/set16Bit.o: src/set16Bit.c src/set16Bit.h
	$(CC) $(CFLAGS) -c -o bin/set16Bit.o src/set16Bit.c
bin/set32Bit.o: src/set32Bit.c src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/set32Bit.o src/set32Bit.c
bin/savePacket.o: src/savePacket.c src/savePacket.h src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/savePacket.o src/savePacket.c
bin/dequeuePacket.o: src/dequeuePacket.c src/dequeuePacket.h src/contrib/heap.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/dequeuePacket.o src/dequeuePacket.c
bin/threadWorker.o: src/threadWorker.c src/threadWorker.h src/contrib/heap.h src/CaptureContext.h src/PacketQueueItem.h src/dequeuePacket.h
	$(CC) $(CFLAGS) -c -o bin/threadWorker.o src/threadWorker.c
bin/compareIPv4FragmentsIdsSources.o: src/compareIPv4FragmentsIdsSources.c src/compareIPv4FragmentsIdsSources.h src/IPv4Fragments.h
	$(CC) $(CFLAGS) -c -o bin/compareIPv4FragmentsIdsSources.o src/compareIPv4FragmentsIdsSources.c
bin/compareTimeoutItems.o: src/compareTimeoutItems.c src/compareTimeoutItems.h src/TimeoutItem.h src/compareTimeval.h
	$(CC) $(CFLAGS) -c -o bin/compareTimeoutItems.o src/compareTimeoutItems.c
bin/compareTimeval.o: src/compareTimeval.c src/compareTimeval.h
	$(CC) $(CFLAGS) -c -o bin/compareTimeval.o src/compareTimeval.c
bin/checksum.a: bin/computeChecksum.o bin/initChecksum.o bin/getChecksum.o
	$(AR) rcs bin/checksum.a bin/computeChecksum.o bin/initChecksum.o bin/getChecksum.o
bin/computeChecksum.o: src/computeChecksum.c src/computeChecksum.h src/ChecksumContext.h src/addShortWithCarry.h
	$(CC) $(CFLAGS) -c -o bin/computeChecksum.o src/computeChecksum.c
bin/initChecksum.o: src/initChecksum.c src/initChecksum.h src/ChecksumContext.h
	$(CC) $(CFLAGS) -c -o bin/initChecksum.o src/initChecksum.c
bin/getChecksum.o: src/getChecksum.c src/getChecksum.h src/ChecksumContext.h
	$(CC) $(CFLAGS) -c -o bin/getChecksum.o src/getChecksum.c
bin/processIPv4Packet.o: src/processIPv4Packet.c src/processIPv4Packet.h src/contrib/heap.h src/CaptureContext.h src/IPv4HeaderData.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/parseIPv4Header.h src/processUDPPacket.h src/setIPv4SockaddrPort.h src/ipv4_strategy.h src/processTCPPacket.h
	$(CC) $(CFLAGS) -c -o bin/processIPv4Packet.o src/processIPv4Packet.c
bin/parseIPv4Header.o: src/parseIPv4Header.c src/parseIPv4Header.h src/IPv4HeaderData.h src/ChecksumContext.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/get16Bit.h src/get32Bit.h
	$(CC) $(CFLAGS) -c -o bin/parseIPv4Header.o src/parseIPv4Header.c
bin/get16Bit.o: src/get16Bit.c src/get16Bit.h
	$(CC) $(CFLAGS) -c -o bin/get16Bit.o src/get16Bit.c
bin/get32Bit.o: src/get32Bit.c src/get32Bit.h
	$(CC) $(CFLAGS) -c -o bin/get32Bit.o src/get32Bit.c
bin/addShortWithCarry.o: src/addShortWithCarry.c src/addShortWithCarry.h
	$(CC) $(CFLAGS) -c -o bin/addShortWithCarry.o src/addShortWithCarry.c
bin/udpCallback.o: src/udpCallback.c src/udpCallback.h src/contrib/heap.h src/UDPQueueItem.h src/UDPBinding.h src/UDPParameters.h src/CaptureContext.h src/PacketQueueItem.h src/HEADERS_RESERVE.h src/MAX_UDP_PAYLOAD.h src/udpGenerator.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/udpCallback.o src/udpCallback.c
bin/compareUDPBindings.o: src/compareUDPBindings.c src/compareUDPBindings.h src/UDPBinding.h
	$(CC) $(CFLAGS) -c -o bin/compareUDPBindings.o src/compareUDPBindings.c
bin/processUDPPacket.o: src/processUDPPacket.c src/processUDPPacket.h src/contrib/heap.h src/CaptureContext.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/UDPHeaderData.h src/UDPQueueItem.h src/ChecksumContext.h src/UDPBinding.h src/get16Bit.h src/set16Bit.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/findUDPBinding.h src/udpCallback.h
	$(CC) $(CFLAGS) -c -o bin/processUDPPacket.o src/processUDPPacket.c
bin/setIPv4SockaddrPort.o: src/setIPv4SockaddrPort.c src/setIPv4SockaddrPort.h
	$(CC) $(CFLAGS) -c -o bin/setIPv4SockaddrPort.o src/setIPv4SockaddrPort.c
bin/findUDPBinding.o: src/findUDPBinding.c src/findUDPBinding.h src/contrib/avl.h src/contrib/heap.h src/CaptureContext.h src/UDPBinding.h src/udpCallback.h
	$(CC) $(CFLAGS) -c -o bin/findUDPBinding.o src/findUDPBinding.c
bin/packetsProcessor.o: src/packetsProcessor.c src/packetsProcessor.h src/contrib/heap.h src/CaptureContext.h src/processIPv4Packet.h
	$(CC) $(CFLAGS) -c -o bin/packetsProcessor.o src/packetsProcessor.c
bin/udpGenerator.o: src/udpGenerator.c src/udpGenerator.h src/contrib/heap.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureSettings.h src/CaptureContext.h src/UDPParameters.h src/UDPBinding.h src/ChecksumContext.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/PacketQueueItem.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/tunCallback.h src/HEADERS_RESERVE.h src/ipv4_strategy.h src/ipv6_strategy.h
	$(CC) $(CFLAGS) -c -o bin/udpGenerator.o src/udpGenerator.c
bin/computeIPv4FragmentsCount.o: src/computeIPv4FragmentsCount.c src/computeIPv4FragmentsCount.h
	$(CC) $(CFLAGS) -c -o bin/computeIPv4FragmentsCount.o src/computeIPv4FragmentsCount.c
bin/ipv4_strategy.o: src/ipv4_strategy.c src/ipv4_strategy.h src/contrib/heap.h src/CaptureContext.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/setIPv4SockaddrPort.h src/getIPv4SockaddrPort.h src/computeIPv4FragmentsCount.h src/fillIPv4FragmentMetadatas.h src/createIPv4PseudoHdr.h src/writeIPv4Headers.h src/computeIPv4MSS.h
	$(CC) $(CFLAGS) -c -o bin/ipv4_strategy.o src/ipv4_strategy.c
bin/fillIPv4FragmentMetadatas.o: src/fillIPv4FragmentMetadatas.c src/fillIPv4FragmentMetadatas.h src/IPFragmentMetadata.h
	$(CC) $(CFLAGS) -c -o bin/fillIPv4FragmentMetadatas.o src/fillIPv4FragmentMetadatas.c
bin/createIPv4PseudoHdr.o: src/createIPv4PseudoHdr.c src/createIPv4PseudoHdr.h
	$(CC) $(CFLAGS) -c -o bin/createIPv4PseudoHdr.o src/createIPv4PseudoHdr.c
bin/writeIPv4Headers.o: src/writeIPv4Headers.c src/writeIPv4Headers.h src/contrib/heap.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureSettings.h src/CaptureContext.h src/IPFragmentMetadata.h src/ChecksumContext.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h
	$(CC) $(CFLAGS) -c -o bin/writeIPv4Headers.o src/writeIPv4Headers.c
bin/getIPv4SockaddrPort.o: src/getIPv4SockaddrPort.c src/getIPv4SockaddrPort.h
	$(CC) $(CFLAGS) -c -o bin/getIPv4SockaddrPort.o src/getIPv4SockaddrPort.c
bin/readPacket.o: src/readPacket.c src/readPacket.h src/UserData.h src/savePacket.h
	$(CC) $(CFLAGS) -c -o bin/readPacket.o src/readPacket.c
bin/writePacket.o: src/writePacket.c src/writePacket.h src/UserData.h src/savePacket.h
	$(CC) $(CFLAGS) -c -o bin/writePacket.o src/writePacket.c
bin/getFdFromUser.o: src/getFdFromUser.c src/getFdFromUser.h src/UserData.h
	$(CC) $(CFLAGS) -c -o bin/getFdFromUser.o src/getFdFromUser.c
bin/timerCallback.o: src/timerCallback.c src/timerCallback.h src/contrib/heap.h src/TimeoutItem.h src/CaptureContext.h src/getMonotonicTimeval.h src/compareTimeval.h src/startTimer.h
	$(CC) $(CFLAGS) -c -o bin/timerCallback.o src/timerCallback.c
bin/addTimeval.o: src/addTimeval.c src/addTimeval.h
	$(CC) $(CFLAGS) -c -o bin/addTimeval.o src/addTimeval.c
bin/subTimeval.o: src/subTimeval.c src/subTimeval.h
	$(CC) $(CFLAGS) -c -o bin/subTimeval.o src/subTimeval.c
bin/getMonotonicTimeval.o: src/getMonotonicTimeval.c src/getMonotonicTimeval.h
	$(CC) $(CFLAGS) -c -o bin/getMonotonicTimeval.o src/getMonotonicTimeval.c
bin/startTimer.o: src/startTimer.c src/startTimer.h src/contrib/heap.h src/TimeoutItem.h src/CaptureContext.h src/getMonotonicTimeval.h src/subTimeval.h
	$(CC) $(CFLAGS) -c -o bin/startTimer.o src/startTimer.c
bin/parseTCPHeader.o: src/parseTCPHeader.c src/parseTCPHeader.h src/TCPHeaderData.h src/ChecksumContext.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/get16Bit.h src/get32Bit.h
	$(CC) $(CFLAGS) -c -o bin/parseTCPHeader.o src/parseTCPHeader.c
bin/processTCPPacket.o: src/processTCPPacket.c src/processTCPPacket.h src/contrib/avl.h src/contrib/heap.h src/CaptureContext.h src/IPPacketPayload.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/SrcDstSockaddrs.h src/TCPHeaderData.h src/TCPState.h src/TCPConnection.h src/parseTCPHeader.h src/tcpCallback.h src/tcpstate_connwait.h
	$(CC) $(CFLAGS) -c -o bin/processTCPPacket.o src/processTCPPacket.c
bin/compareTCPConnections.o: src/compareTCPConnections.c src/compareTCPConnections.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) -c -o bin/compareTCPConnections.o src/compareTCPConnections.c
bin/tcpCallback.o: src/tcpCallback.c src/tcpCallback.h src/SrcDstSockaddrs.h src/IPPacketPayload.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h
	$(CC) $(CFLAGS) -c -o bin/tcpCallback.o src/tcpCallback.c
bin/tcpstate_connwait.o: src/tcpstate_connwait.c src/tcpstate_connwait.h src/SrcDstSockaddrs.h src/IPPacketPayload.h src/TCPHeaderData.h src/TCPState.h src/tcpConnwaitEventCallback.h src/tcpConnwaitPacketsProcessor.h
	$(CC) $(CFLAGS) -c -o bin/tcpstate_connwait.o src/tcpstate_connwait.c
bin/tcpConnwaitEventCallback.o: src/tcpConnwaitEventCallback.c src/tcpConnwaitEventCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h src/destroyTCPConnection.h src/tcpstate_synack_send.h
	$(CC) $(CFLAGS) -c -o bin/tcpConnwaitEventCallback.o src/tcpConnwaitEventCallback.c
bin/destroyTCPConnection.o: src/destroyTCPConnection.c src/destroyTCPConnection.h src/contrib/avl.h src/contrib/heap.h src/SrcDstSockaddrs.h src/CaptureContext.h src/TCPConnection.h
	$(CC) $(CFLAGS) -c -o bin/destroyTCPConnection.o src/destroyTCPConnection.c
bin/tcpConnwaitPacketsProcessor.o: src/tcpConnwaitPacketsProcessor.c src/tcpConnwaitPacketsProcessor.h src/IPPacketPayload.h src/TCPHeaderData.h
	$(CC) $(CFLAGS) -c -o bin/tcpConnwaitPacketsProcessor.o src/tcpConnwaitPacketsProcessor.c
bin/writeTCPHeader.o: src/writeTCPHeader.c src/writeTCPHeader.h src/ChecksumContext.h src/TCPHeaderData.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/set16Bit.h src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/writeTCPHeader.o src/writeTCPHeader.c
bin/tcpSynackSendEventCallback.o: src/tcpSynackSendEventCallback.c src/tcpSynackSendEventCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) -c -o bin/tcpSynackSendEventCallback.o src/tcpSynackSendEventCallback.c
bin/tcpSynackSendPacketsProcessor.o: src/tcpSynackSendPacketsProcessor.c src/tcpSynackSendPacketsProcessor.h src/IPPacketPayload.h src/TCPHeaderData.h
	$(CC) $(CFLAGS) -c -o bin/tcpSynackSendPacketsProcessor.o src/tcpSynackSendPacketsProcessor.c
bin/tcpstate_synack_send.o: src/tcpstate_synack_send.c src/tcpstate_synack_send.h src/SrcDstSockaddrs.h src/IPPacketPayload.h src/TCPHeaderData.h src/TCPState.h src/tcpConnwaitEventCallback.h src/tcpConnwaitPacketsProcessor.h
	$(CC) $(CFLAGS) -c -o bin/tcpstate_synack_send.o src/tcpstate_synack_send.c
bin/getSendWindowSize.o: src/getSendWindowSize.c src/getSendWindowSize.h src/SrcDstSockaddrs.h src/TCPSiteQueueItem.h src/TCPConnection.h src/MAX_SITE_QUEUE.h
	$(CC) $(CFLAGS) -c -o bin/getSendWindowSize.o src/getSendWindowSize.c
bin/computeIPv4MSS.o: src/computeIPv4MSS.c src/computeIPv4MSS.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h
	$(CC) $(CFLAGS) -c -o bin/computeIPv4MSS.o src/computeIPv4MSS.c
bin/computeIPv6MSS.o: src/computeIPv6MSS.c src/computeIPv6MSS.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h
	$(CC) $(CFLAGS) -c -o bin/computeIPv6MSS.o src/computeIPv6MSS.c
bin/sendSynReply.o: src/sendSynReply.c src/sendSynReply.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h src/SrcDstSockaddrs.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/TCPAppQueueItem.h src/TCPHeaderData.h src/TCPConnection.h src/getSendWindowSize.h src/writeTCPHeader.h src/startTimer.h src/enqueueTimeout.h src/tcpRetransmissionTimerCallback.h src/sendTCPPacket.h src/getMonotonicTimeval.h src/addTimeval.h src/computeTCPDataOffset.h src/retry_delay.h src/HEADERS_RESERVE.h
	$(CC) $(CFLAGS) -c -o bin/sendSynReply.o src/sendSynReply.c
bin/retry_delay.o: src/retry_delay.c src/retry_delay.h
	$(CC) $(CFLAGS) -c -o bin/retry_delay.o src/retry_delay.c
bin/enqueueTimeout.o: src/enqueueTimeout.c src/enqueueTimeout.h src/contrib/heap.h src/CaptureContext.h src/TimeoutItem.h
	$(CC) $(CFLAGS) -c -o bin/enqueueTimeout.o src/enqueueTimeout.c
bin/sendTCPPacket.o: src/sendTCPPacket.c src/sendTCPPacket.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h src/PacketQueueItem.h src/TCPAppQueueItem.h src/tunCallback.h
	$(CC) $(CFLAGS) -c -o bin/sendTCPPacket.o src/sendTCPPacket.c
bin/tcpRetransmissionTimerCallback.o: src/tcpRetransmissionTimerCallback.c src/tcpRetransmissionTimerCallback.h src/contrib/heap.h src/CaptureContext.h src/SrcDstSockaddrs.h src/TCPAppQueueItem.h src/TCPConnection.h src/getMonotonicTimeval.h src/compareTimeval.h src/sendTCPPacket.h src/addTimeval.h src/compareTimeval.h src/enqueueTimeout.h src/retry_delay.h
	$(CC) $(CFLAGS) -c -o bin/tcpRetransmissionTimerCallback.o src/tcpRetransmissionTimerCallback.c
bin/computeTCPDataOffset.o: src/computeTCPDataOffset.c src/computeTCPDataOffset.h src/TCPHeaderData.h
	$(CC) $(CFLAGS) -c -o bin/computeTCPDataOffset.o src/computeTCPDataOffset.c
bin/avl.o: src/contrib/avl.c src/contrib/avl.h
	$(CC) $(CFLAGS) -c -o bin/avl.o src/contrib/avl.c
bin/heap.o: src/contrib/heap.c src/contrib/heap.h
	$(CC) $(CFLAGS) -c -o bin/heap.o src/contrib/heap.c
clean:
	rm -f bin/*

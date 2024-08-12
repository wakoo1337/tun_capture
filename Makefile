LIBEVENT_LIBS := -levent -levent_pthreads
LIBEVENT_CFLAGS := 
AR := ar
CC := cc
CFLAGS := -O0 -g -Wall -std=gnu11
.PHONY: all
all: bin/tun_capture
bin/tun_capture: bin/tun_capture.o bin/openInterface.o bin/doCapture.o bin/tunReadCallback.o bin/set16Bit.o bin/set32Bit.o bin/savePacket.o bin/dequeueRxPacket.o bin/threadWorker.o bin/compareIPv4FragmentsIdsSources.o bin/compareTimeoutItems.o bin/processIPv4Packet.o bin/compareTimeval.o bin/get16Bit.o bin/get32Bit.o bin/parseIPv4Header.o bin/addShortWithCarry.o bin/udpReadCallback.o bin/udpWriteCallback.o bin/compareUDPBindings.o bin/processUDPPacket.o bin/setIPv4SockaddrPort.o bin/findUDPBinding.o bin/packetsProcessor.o bin/udpGenerator.o bin/computeIPv4FragmentsCount.o bin/ipv4_strategy.o bin/fillIPv4FragmentMetadatas.o bin/createIPv4PseudoHdr.o bin/writeIPv4Headers.o bin/getIPv4SockaddrPort.o bin/readPacket.o bin/writePacket.o bin/getFdFromUser.o bin/addTimeval.o bin/subTimeval.o bin/getMonotonicTimeval.o bin/timerCallback.o bin/startTimer.o bin/parseTCPHeader.o bin/processTCPPacket.o bin/compareTCPConnections.o bin/tcpReadCallback.o bin/tcpstate_connwait.o bin/tcpConnwaitReadCallback.o bin/tcpConnwaitWriteCallback.o bin/destroyTCPConnection.o bin/tcpConnwaitPacketsProcessor.o bin/writeTCPHeader.o bin/tcpSynackSendReadCallback.o bin/tcpSynackSendWriteCallback.o bin/tcpSynackSendPacketsProcessor.o bin/tcpstate_synack_send.o bin/getSendWindowSize.o bin/computeIPv4MSS.o bin/computeIPv6MSS.o bin/sendSynReply.o bin/retry_delay.o bin/enqueueTimeout.o bin/enqueueTCPPacketTransmission.o bin/tcpRetransmissionTimerCallback.o bin/computeTCPDataOffset.o bin/enqueueRxPacket.o bin/tcpstate_established.o bin/tcpEstablishedReadCallback.o bin/tcpEstablishedWriteCallback.o bin/tcpEstablishedPacketsProcessor.o bin/cancelTimeout.o bin/tcpCleanupConfirmed.o bin/tcpUpdateReadEvent.o bin/tcpUpdateWriteEvent.o bin/checkByteInWindow.o bin/compareTCPSitePrequeueItems.o bin/segexpire_delay.o bin/tcpDeleteExpiredSegment.o bin/enqueueSiteDataFromPrequeueItem.o bin/sendTCPAcknowledgement.o bin/processTCPUrgentData.o bin/processTCPData.o bin/dequeueTxPacket.o bin/tunWriteCallback.o bin/emergencyStop.o bin/enqueueTxPacket.o bin/tcpWriteCallback.o bin/tcpDestroySitePrequeueItem.o bin/tcpFinalizeRead.o bin/tcpFinalizeWrite.o bin/tcpstate_gotfin.o bin/sendPacketOnce.o bin/sendTCPPacketRefcounted.o bin/tcpGotFINReadCallback.o bin/tcpGotFINWriteCallback.o bin/tcpGotFINPacketsProcessor.o bin/sendSiteQueueItems.o bin/checksum.a bin/avl.o bin/heap.o
	$(CC) $(CFLAGS) -o bin/tun_capture bin/tun_capture.o bin/openInterface.o bin/doCapture.o bin/tunReadCallback.o bin/set16Bit.o bin/set32Bit.o bin/savePacket.o bin/dequeueRxPacket.o bin/threadWorker.o bin/compareIPv4FragmentsIdsSources.o bin/compareTimeoutItems.o bin/compareTimeval.o bin/processIPv4Packet.o bin/get16Bit.o bin/get32Bit.o bin/parseIPv4Header.o bin/addShortWithCarry.o bin/udpReadCallback.o bin/udpWriteCallback.o bin/compareUDPBindings.o bin/processUDPPacket.o bin/setIPv4SockaddrPort.o bin/findUDPBinding.o bin/packetsProcessor.o bin/udpGenerator.o bin/computeIPv4FragmentsCount.o bin/ipv4_strategy.o bin/fillIPv4FragmentMetadatas.o bin/createIPv4PseudoHdr.o bin/writeIPv4Headers.o bin/getIPv4SockaddrPort.o bin/readPacket.o bin/writePacket.o bin/getFdFromUser.o bin/addTimeval.o bin/subTimeval.o bin/getMonotonicTimeval.o bin/timerCallback.o bin/startTimer.o bin/parseTCPHeader.o bin/processTCPPacket.o bin/compareTCPConnections.o bin/tcpReadCallback.o bin/tcpstate_connwait.o bin/tcpConnwaitReadCallback.o bin/tcpConnwaitWriteCallback.o bin/destroyTCPConnection.o bin/tcpConnwaitPacketsProcessor.o bin/writeTCPHeader.o bin/tcpSynackSendReadCallback.o bin/tcpSynackSendWriteCallback.o bin/tcpSynackSendPacketsProcessor.o bin/tcpstate_synack_send.o bin/getSendWindowSize.o bin/computeIPv4MSS.o bin/computeIPv6MSS.o bin/sendSynReply.o bin/retry_delay.o bin/enqueueTimeout.o bin/enqueueTCPPacketTransmission.o bin/tcpRetransmissionTimerCallback.o bin/computeTCPDataOffset.o bin/enqueueRxPacket.o bin/tcpstate_established.o bin/tcpEstablishedReadCallback.o bin/tcpEstablishedWriteCallback.o bin/tcpEstablishedPacketsProcessor.o bin/cancelTimeout.o bin/tcpCleanupConfirmed.o bin/tcpUpdateReadEvent.o bin/tcpUpdateWriteEvent.o bin/checkByteInWindow.o bin/compareTCPSitePrequeueItems.o bin/segexpire_delay.o bin/tcpDeleteExpiredSegment.o bin/enqueueSiteDataFromPrequeueItem.o bin/sendTCPAcknowledgement.o bin/processTCPUrgentData.o bin/processTCPData.o bin/dequeueTxPacket.o bin/tunWriteCallback.o bin/emergencyStop.o bin/enqueueTxPacket.o bin/tcpWriteCallback.o bin/tcpDestroySitePrequeueItem.o bin/tcpFinalizeRead.o bin/tcpFinalizeWrite.o bin/tcpstate_gotfin.o bin/sendPacketOnce.o bin/sendTCPPacketRefcounted.o bin/tcpGotFINReadCallback.o bin/tcpGotFINWriteCallback.o bin/tcpGotFINPacketsProcessor.o bin/sendSiteQueueItems.o bin/checksum.a bin/avl.o bin/heap.o $(LIBEVENT_LIBS) -lpthread
bin/tun_capture.o: src/tun_capture.c src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/UserData.h src/CaptureSettings.h src/openInterface.h src/doCapture.h src/set32Bit.h src/set16Bit.h src/getFdFromUser.h src/readPacket.h src/writePacket.h
	$(CC) $(CFLAGS) -c -o bin/tun_capture.o src/tun_capture.c
bin/openInterface.o: src/openInterface.c src/openInterface.h
	$(CC) $(CFLAGS) -c -o bin/openInterface.o src/openInterface.c
bin/doCapture.o: src/doCapture.c src/doCapture.h src/contrib/avl.h src/contrib/heap.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureContext.h src/CaptureSettings.h src/tunReadCallback.h src/tunWriteCallback.h src/threadWorker.h src/compareIPv4FragmentsIdsSources.h src/compareTimeoutItems.h src/compareUDPBindings.h src/UDPBinding.h src/timerCallback.h src/compareTCPConnections.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/doCapture.o src/doCapture.c
bin/tunReadCallback.o: src/tunReadCallback.c src/tunReadCallback.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h src/PacketQueueItem.h src/enqueueRxPacket.h src/packetsProcessor.h src/emergencyStop.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tunReadCallback.o src/tunReadCallback.c
bin/set16Bit.o: src/set16Bit.c src/set16Bit.h
	$(CC) $(CFLAGS) -c -o bin/set16Bit.o src/set16Bit.c
bin/set32Bit.o: src/set32Bit.c src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/set32Bit.o src/set32Bit.c
bin/savePacket.o: src/savePacket.c src/savePacket.h src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/savePacket.o src/savePacket.c
bin/dequeueRxPacket.o: src/dequeueRxPacket.c src/dequeueRxPacket.h src/contrib/heap.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/dequeueRxPacket.o src/dequeueRxPacket.c
bin/threadWorker.o: src/threadWorker.c src/threadWorker.h src/contrib/heap.h src/CaptureContext.h src/PacketQueueItem.h src/dequeueRxPacket.h src/emergencyStop.h
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
bin/udpReadCallback.o: src/udpReadCallback.c src/udpReadCallback.h src/contrib/heap.h src/CaptureContext.h src/PacketQueueItem.h src/UDPParameters.h src/UDPStackItem.h src/UDPBinding.h src/udpGenerator.h src/emergencyStop.h src/enqueueRxPacket.h src/HEADERS_RESERVE.h src/MAX_UDP_PAYLOAD.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/udpReadCallback.o src/udpReadCallback.c
bin/compareUDPBindings.o: src/compareUDPBindings.c src/compareUDPBindings.h src/UDPBinding.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/compareUDPBindings.o src/compareUDPBindings.c
bin/processUDPPacket.o: src/processUDPPacket.c src/processUDPPacket.h src/contrib/heap.h src/CaptureContext.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/UDPHeaderData.h src/UDPStackItem.h src/ChecksumContext.h src/UDPBinding.h src/get16Bit.h src/set16Bit.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/findUDPBinding.h src/udpReadCallback.h src/udpWriteCallback.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/processUDPPacket.o src/processUDPPacket.c
bin/setIPv4SockaddrPort.o: src/setIPv4SockaddrPort.c src/setIPv4SockaddrPort.h
	$(CC) $(CFLAGS) -c -o bin/setIPv4SockaddrPort.o src/setIPv4SockaddrPort.c
bin/findUDPBinding.o: src/findUDPBinding.c src/findUDPBinding.h src/contrib/avl.h src/contrib/heap.h src/CaptureContext.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/UDPBinding.h src/udpReadCallback.h src/udpWriteCallback.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/findUDPBinding.o src/findUDPBinding.c
bin/packetsProcessor.o: src/packetsProcessor.c src/packetsProcessor.h src/contrib/heap.h src/CaptureContext.h src/processIPv4Packet.h
	$(CC) $(CFLAGS) -c -o bin/packetsProcessor.o src/packetsProcessor.c
bin/udpGenerator.o: src/udpGenerator.c src/udpGenerator.h src/contrib/heap.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureSettings.h src/CaptureContext.h src/UDPParameters.h src/UDPBinding.h src/ChecksumContext.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/PacketQueueItem.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/set16Bit.h src/enqueueTxPacket.h src/sendPacketOnce.h src/HEADERS_RESERVE.h src/ipv4_strategy.h src/ipv6_strategy.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/udpGenerator.o src/udpGenerator.c
bin/computeIPv4FragmentsCount.o: src/computeIPv4FragmentsCount.c src/computeIPv4FragmentsCount.h
	$(CC) $(CFLAGS) -c -o bin/computeIPv4FragmentsCount.o src/computeIPv4FragmentsCount.c
bin/ipv4_strategy.o: src/ipv4_strategy.c src/ipv4_strategy.h src/contrib/heap.h src/CaptureContext.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/setIPv4SockaddrPort.h src/getIPv4SockaddrPort.h src/computeIPv4FragmentsCount.h src/fillIPv4FragmentMetadatas.h src/createIPv4PseudoHdr.h src/writeIPv4Headers.h src/computeIPv4MSS.h
	$(CC) $(CFLAGS) -c -o bin/ipv4_strategy.o src/ipv4_strategy.c
bin/fillIPv4FragmentMetadatas.o: src/fillIPv4FragmentMetadatas.c src/fillIPv4FragmentMetadatas.h src/IPFragmentMetadata.h
	$(CC) $(CFLAGS) -c -o bin/fillIPv4FragmentMetadatas.o src/fillIPv4FragmentMetadatas.c
bin/createIPv4PseudoHdr.o: src/createIPv4PseudoHdr.c src/createIPv4PseudoHdr.h
	$(CC) $(CFLAGS) -c -o bin/createIPv4PseudoHdr.o src/createIPv4PseudoHdr.c
bin/writeIPv4Headers.o: src/writeIPv4Headers.c src/writeIPv4Headers.h src/contrib/heap.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureSettings.h src/CaptureContext.h src/IPFragmentMetadata.h src/ChecksumContext.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/set16Bit.h
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
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/timerCallback.o src/timerCallback.c
bin/addTimeval.o: src/addTimeval.c src/addTimeval.h
	$(CC) $(CFLAGS) -c -o bin/addTimeval.o src/addTimeval.c
bin/subTimeval.o: src/subTimeval.c src/subTimeval.h
	$(CC) $(CFLAGS) -c -o bin/subTimeval.o src/subTimeval.c
bin/getMonotonicTimeval.o: src/getMonotonicTimeval.c src/getMonotonicTimeval.h
	$(CC) $(CFLAGS) -c -o bin/getMonotonicTimeval.o src/getMonotonicTimeval.c
bin/startTimer.o: src/startTimer.c src/startTimer.h src/contrib/heap.h src/TimeoutItem.h src/CaptureContext.h src/getMonotonicTimeval.h src/subTimeval.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/startTimer.o src/startTimer.c
bin/parseTCPHeader.o: src/parseTCPHeader.c src/parseTCPHeader.h src/TCPHeaderData.h src/ChecksumContext.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/get16Bit.h src/get32Bit.h
	$(CC) $(CFLAGS) -c -o bin/parseTCPHeader.o src/parseTCPHeader.c
bin/processTCPPacket.o: src/processTCPPacket.c src/processTCPPacket.h src/contrib/avl.h src/contrib/heap.h src/CaptureContext.h src/IPPacketPayload.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/SrcDstSockaddrs.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/parseTCPHeader.h src/compareTCPSitePrequeueItems.h src/tcpReadCallback.h src/tcpWriteCallback.h src/tcpDestroySitePrequeueItem.h src/tcpFinalizeRead.h src/tcpFinalizeWrite.h src/tcpstate_connwait.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/processTCPPacket.o src/processTCPPacket.c
bin/compareTCPConnections.o: src/compareTCPConnections.c src/compareTCPConnections.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) -c -o bin/compareTCPConnections.o src/compareTCPConnections.c
bin/tcpReadCallback.o: src/tcpReadCallback.c src/tcpReadCallback.h src/contrib/heap.h src/CaptureContext.h src/SrcDstSockaddrs.h src/IPPacketPayload.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/emergencyStop.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpReadCallback.o src/tcpReadCallback.c
bin/tcpstate_connwait.o: src/tcpstate_connwait.c src/tcpstate_connwait.h src/SrcDstSockaddrs.h src/IPPacketPayload.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/tcpConnwaitReadCallback.h src/tcpConnwaitWriteCallback.h src/tcpConnwaitPacketsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_connwait.o src/tcpstate_connwait.c
bin/tcpConnwaitReadCallback.o: src/tcpConnwaitReadCallback.c src/tcpConnwaitReadCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpConnwaitReadCallback.o src/tcpConnwaitReadCallback.c
bin/tcpConnwaitWriteCallback.o: src/tcpConnwaitWriteCallback.c src/tcpConnwaitWriteCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h src/destroyTCPConnection.h src/sendSynReply.h src/tcpstate_synack_send.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpConnwaitWriteCallback.o src/tcpConnwaitWriteCallback.c
bin/destroyTCPConnection.o: src/destroyTCPConnection.c src/destroyTCPConnection.h src/contrib/avl.h src/contrib/heap.h src/SrcDstSockaddrs.h src/CaptureContext.h src/TCPConnection.h src/tcpDestroySitePrequeueItem.h
	$(CC) $(CFLAGS) -c -o bin/destroyTCPConnection.o src/destroyTCPConnection.c
bin/tcpConnwaitPacketsProcessor.o: src/tcpConnwaitPacketsProcessor.c src/tcpConnwaitPacketsProcessor.h src/IPPacketPayload.h src/TCPHeaderData.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) -c -o bin/tcpConnwaitPacketsProcessor.o src/tcpConnwaitPacketsProcessor.c
bin/writeTCPHeader.o: src/writeTCPHeader.c src/writeTCPHeader.h src/ChecksumContext.h src/TCPHeaderData.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/set16Bit.h src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/writeTCPHeader.o src/writeTCPHeader.c
bin/tcpSynackSendReadCallback.o: src/tcpSynackSendReadCallback.c src/tcpSynackSendReadCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpSynackSendReadCallback.o src/tcpSynackSendReadCallback.c
bin/tcpSynackSendWriteCallback.o: src/tcpSynackSendWriteCallback.c src/tcpSynackSendWriteCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpSynackSendWriteCallback.o src/tcpSynackSendWriteCallback.c
bin/tcpSynackSendPacketsProcessor.o: src/tcpSynackSendPacketsProcessor.c src/tcpSynackSendPacketsProcessor.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPHeaderData.h src/TimeoutItem.h src/tcpCleanupConfirmed.h src/tcpUpdateReadEvent.h src/tcpUpdateWriteEvent.h src/tcpstate_established.h
	$(CC) $(CFLAGS) -c -o bin/tcpSynackSendPacketsProcessor.o src/tcpSynackSendPacketsProcessor.c
bin/tcpstate_synack_send.o: src/tcpstate_synack_send.c src/tcpstate_synack_send.h src/SrcDstSockaddrs.h src/IPPacketPayload.h src/TCPConnection.h src/TCPHeaderData.h src/TCPState.h src/tcpSynackSendReadCallback.h src/tcpSynackSendWriteCallback.h src/tcpSynackSendPacketsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_synack_send.o src/tcpstate_synack_send.c
bin/getSendWindowSize.o: src/getSendWindowSize.c src/getSendWindowSize.h src/SrcDstSockaddrs.h src/TCPSiteQueueItem.h src/TCPConnection.h src/MAX_SITE_QUEUE.h
	$(CC) $(CFLAGS) -c -o bin/getSendWindowSize.o src/getSendWindowSize.c
bin/computeIPv4MSS.o: src/computeIPv4MSS.c src/computeIPv4MSS.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h
	$(CC) $(CFLAGS) -c -o bin/computeIPv4MSS.o src/computeIPv4MSS.c
bin/computeIPv6MSS.o: src/computeIPv6MSS.c src/computeIPv6MSS.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h
	$(CC) $(CFLAGS) -c -o bin/computeIPv6MSS.o src/computeIPv6MSS.c
bin/sendSynReply.o: src/sendSynReply.c src/sendSynReply.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h src/SrcDstSockaddrs.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/TCPAppQueueItem.h src/TCPHeaderData.h src/TCPConnection.h src/getSendWindowSize.h src/writeTCPHeader.h src/startTimer.h src/enqueueTimeout.h src/tcpRetransmissionTimerCallback.h src/enqueueTCPPacketTransmission.h src/getMonotonicTimeval.h src/addTimeval.h src/computeTCPDataOffset.h src/retry_delay.h src/HEADERS_RESERVE.h
	$(CC) $(CFLAGS) -c -o bin/sendSynReply.o src/sendSynReply.c
bin/retry_delay.o: src/retry_delay.c src/retry_delay.h
	$(CC) $(CFLAGS) -c -o bin/retry_delay.o src/retry_delay.c
bin/enqueueTimeout.o: src/enqueueTimeout.c src/enqueueTimeout.h src/contrib/heap.h src/CaptureContext.h src/TimeoutItem.h
	$(CC) $(CFLAGS) -c -o bin/enqueueTimeout.o src/enqueueTimeout.c
bin/enqueueTCPPacketTransmission.o: src/enqueueTCPPacketTransmission.c src/enqueueTCPPacketTransmission.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h src/PacketQueueItem.h src/TCPAppQueueItem.h src/SrcDstSockaddrs.h src/TCPConnection.h src/enqueueTxPacket.h src/sendTCPPacketRefcounted.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/enqueueTCPPacketTransmission.o src/enqueueTCPPacketTransmission.c
bin/tcpRetransmissionTimerCallback.o: src/tcpRetransmissionTimerCallback.c src/tcpRetransmissionTimerCallback.h src/contrib/heap.h src/SrcDstSockaddrs.h src/CaptureContext.h src/TCPConnection.h src/TCPAppQueueItem.h src/getMonotonicTimeval.h src/compareTimeval.h src/enqueueTCPPacketTransmission.h src/addTimeval.h src/checkByteInWindow.h src/enqueueTimeout.h src/retry_delay.h
	$(CC) $(CFLAGS) -c -o bin/tcpRetransmissionTimerCallback.o src/tcpRetransmissionTimerCallback.c
bin/computeTCPDataOffset.o: src/computeTCPDataOffset.c src/computeTCPDataOffset.h src/TCPHeaderData.h
	$(CC) $(CFLAGS) -c -o bin/computeTCPDataOffset.o src/computeTCPDataOffset.c
bin/enqueueRxPacket.o: src/enqueueRxPacket.c src/enqueueRxPacket.h src/contrib/heap.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/enqueueRxPacket.o src/enqueueRxPacket.c
bin/tcpstate_established.o: src/tcpstate_established.c src/tcpstate_established.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/tcpEstablishedReadCallback.h src/tcpEstablishedWriteCallback.h src/tcpEstablishedPacketsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_established.o src/tcpstate_established.c
bin/tcpEstablishedReadCallback.o: src/tcpEstablishedReadCallback.c src/tcpEstablishedReadCallback.h src/contrib/heap.h src/CaptureContext.h src/SrcDstSockaddrs.h src/TCPSiteQueueItem.h src/TCPConnection.h src/PacketQueueItem.h src/tcpUpdateReadEvent.h src/processTCPUrgentData.h src/processTCPData.h src/enqueueRxPacket.h src/tcpUpdateReadEvent.h src/HEADERS_RESERVE.h src/MAX_APP_QUEUE.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpEstablishedReadCallback.o src/tcpEstablishedReadCallback.c
bin/tcpEstablishedWriteCallback.o: src/tcpEstablishedWriteCallback.c src/tcpEstablishedWriteCallback.h src/SrcDstSockaddrs.h src/TCPSiteQueueItem.h src/TCPConnection.h src/getSendWindowSize.h src/sendTCPAcknowledgement.h src/sendSiteQueueItems.h src/tcpUpdateWriteEvent.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpEstablishedWriteCallback.o src/tcpEstablishedWriteCallback.c
bin/tcpEstablishedPacketsProcessor.o: src/tcpEstablishedPacketsProcessor.c src/tcpEstablishedPacketsProcessor.h src/contrib/avl.h src/contrib/heap.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPHeaderData.h src/TCPSitePrequeueItem.h src/CaptureContext.h src/getMonotonicTimeval.h src/addTimeval.h src/checkByteInWindow.h src/enqueueTimeout.h src/tcpDeleteExpiredSegment.h src/enqueueSiteDataFromPrequeueItem.h src/tcpUpdateReadEvent.h src/tcpUpdateWriteEvent.h src/cancelTimeout.h src/startTimer.h src/sendTCPAcknowledgement.h src/tcpCleanupConfirmed.h src/segexpire_delay.h src/tcpstate_gotfin.h src/MAX_SITE_QUEUE.h
	$(CC) $(CFLAGS) -c -o bin/tcpEstablishedPacketsProcessor.o src/tcpEstablishedPacketsProcessor.c
bin/cancelTimeout.o: src/cancelTimeout.c src/cancelTimeout.h src/contrib/heap.h src/CaptureContext.h src/TimeoutItem.h
	$(CC) $(CFLAGS) -c -o bin/cancelTimeout.o src/cancelTimeout.c
bin/tcpCleanupConfirmed.o: src/tcpCleanupConfirmed.c src/tcpCleanupConfirmed.h src/contrib/heap.h src/SrcDstSockaddrs.h src/CaptureContext.h src/TCPAppQueueItem.h src/TCPConnection.h src/cancelTimeout.h
	$(CC) $(CFLAGS) -c -o bin/tcpCleanupConfirmed.o src/tcpCleanupConfirmed.c
bin/tcpUpdateReadEvent.o: src/tcpUpdateReadEvent.c src/tcpUpdateReadEvent.h src/SrcDstSockaddrs.h src/TCPConnection.h src/MAX_APP_QUEUE.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpUpdateReadEvent.o src/tcpUpdateReadEvent.c
bin/tcpUpdateWriteEvent.o: src/tcpUpdateWriteEvent.c src/tcpUpdateWriteEvent.h src/SrcDstSockaddrs.h src/TCPConnection.h src/MAX_APP_QUEUE.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpUpdateWriteEvent.o src/tcpUpdateWriteEvent.c
bin/checkByteInWindow.o: src/checkByteInWindow.c src/checkByteInWindow.h
	$(CC) $(CFLAGS) -c -o bin/checkByteInWindow.o src/checkByteInWindow.c
bin/compareTCPSitePrequeueItems.o: src/compareTCPSitePrequeueItems.c src/compareTCPSitePrequeueItems.h src/TCPSitePrequeueItem.h
	$(CC) $(CFLAGS) -c -o bin/compareTCPSitePrequeueItems.o src/compareTCPSitePrequeueItems.c
bin/segexpire_delay.o: src/segexpire_delay.c src/segexpire_delay.h
	$(CC) $(CFLAGS) -c -o bin/segexpire_delay.o src/segexpire_delay.c
bin/tcpDeleteExpiredSegment.o: src/tcpDeleteExpiredSegment.c src/tcpDeleteExpiredSegment.h src/contrib/avl.h src/TimeoutItem.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPSitePrequeueItem.h src/cancelTimeout.h
	$(CC) $(CFLAGS) -c -o bin/tcpDeleteExpiredSegment.o src/tcpDeleteExpiredSegment.c
bin/enqueueSiteDataFromPrequeueItem.o: src/enqueueSiteDataFromPrequeueItem.c src/enqueueSiteDataFromPrequeueItem.h src/contrib/avl.h src/SrcDstSockaddrs.h src/TCPSitePrequeueItem.h src/TCPSiteQueueItem.h src/TCPConnection.h
	$(CC) $(CFLAGS) -c -o bin/enqueueSiteDataFromPrequeueItem.o src/enqueueSiteDataFromPrequeueItem.c
bin/sendTCPAcknowledgement.o: src/sendTCPAcknowledgement.c src/sendTCPAcknowledgement.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h src/TCPHeaderData.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPAppQueueItem.h src/writeTCPHeader.h src/enqueueTCPPacketTransmission.h src/getSendWindowSize.h src/computeTCPDataOffset.h src/HEADERS_RESERVE.h
	$(CC) $(CFLAGS) -c -o bin/sendTCPAcknowledgement.o src/sendTCPAcknowledgement.c
bin/processTCPUrgentData.o: src/processTCPUrgentData.c src/processTCPUrgentData.h src/contrib/heap.h src/CaptureContext.h
	$(CC) $(CFLAGS) -c -o bin/processTCPUrgentData.o src/processTCPUrgentData.c
bin/processTCPData.o: src/processTCPData.c src/processTCPData.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h src/SrcDstSockaddrs.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/TCPConnection.h src/TCPAppQueueItem.h src/TCPHeaderData.h src/getSendWindowSize.h src/computeTCPDataOffset.h src/writeTCPHeader.h src/checkByteInWindow.h src/enqueueTCPPacketTransmission.h src/tcpRetransmissionTimerCallback.h src/getMonotonicTimeval.h src/addTimeval.h src/enqueueTimeout.h src/startTimer.h src/retry_delay.h src/HEADERS_RESERVE.h
	$(CC) $(CFLAGS) -c -o bin/processTCPData.o src/processTCPData.c
bin/dequeueTxPacket.o: src/dequeueTxPacket.c src/dequeueTxPacket.h src/contrib/heap.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/dequeueTxPacket.o src/dequeueTxPacket.c
bin/tunWriteCallback.o: src/tunWriteCallback.c src/tunWriteCallback.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h src/dequeueTxPacket.h src/emergencyStop.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tunWriteCallback.o src/tunWriteCallback.c
bin/emergencyStop.o: src/emergencyStop.c src/emergencyStop.h src/contrib/heap.h src/CaptureContext.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/emergencyStop.o src/emergencyStop.c
bin/udpWriteCallback.o: src/udpWriteCallback.c src/udpWriteCallback.h src/contrib/heap.h src/CaptureContext.h src/UDPBinding.h src/UDPStackItem.h src/emergencyStop.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/udpWriteCallback.o src/udpWriteCallback.c
bin/enqueueTxPacket.o: src/enqueueTxPacket.c src/enqueueTxPacket.h src/contrib/heap.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/enqueueTxPacket.o src/enqueueTxPacket.c
bin/tcpWriteCallback.o: src/tcpWriteCallback.c src/tcpWriteCallback.h src/contrib/heap.h src/CaptureContext.h src/SrcDstSockaddrs.h src/IPPacketPayload.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/emergencyStop.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpWriteCallback.o src/tcpWriteCallback.c
bin/tcpDestroySitePrequeueItem.o: src/tcpDestroySitePrequeueItem.c src/tcpDestroySitePrequeueItem.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPSitePrequeueItem.h src/cancelTimeout.h
	$(CC) $(CFLAGS) -c -o bin/tcpDestroySitePrequeueItem.o src/tcpDestroySitePrequeueItem.c
bin/tcpFinalizeRead.o: src/tcpFinalizeRead.c src/tcpFinalizeRead.h src/SrcDstSockaddrs.h src/TCPConnection.h src/destroyTCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpFinalizeRead.o src/tcpFinalizeRead.c
bin/tcpFinalizeWrite.o: src/tcpFinalizeWrite.c src/tcpFinalizeWrite.h src/SrcDstSockaddrs.h src/TCPConnection.h src/destroyTCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpFinalizeWrite.o src/tcpFinalizeWrite.c
bin/tcpstate_gotfin.o: src/tcpstate_gotfin.c src/tcpstate_gotfin.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_gotfin.o src/tcpstate_gotfin.c
bin/sendPacketOnce.o: src/sendPacketOnce.c src/sendPacketOnce.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/sendPacketOnce.o src/sendPacketOnce.c
bin/sendTCPPacketRefcounted.o: src/sendTCPPacketRefcounted.c src/sendTCPPacketRefcounted.h src/contrib/heap.h src/CaptureSettings.h src/CaptureContext.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPAppQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/sendTCPPacketRefcounted.o src/sendTCPPacketRefcounted.c
bin/tcpGotFINReadCallback.o: src/tcpGotFINReadCallback.c src/tcpGotFINReadCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpGotFINReadCallback.o src/tcpGotFINReadCallback.c
bin/tcpGotFINWriteCallback.o: src/tcpGotFINWriteCallback.c src/tcpGotFINWriteCallback.h src/TCPSiteQueueItem.h src/SrcDstSockaddrs.h src/TCPConnection.h src/sendSiteQueueItems.h src/tcpFinalizeWrite.h src/tcpUpdateWriteEvent.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpGotFINWriteCallback.o src/tcpGotFINWriteCallback.c
bin/tcpGotFINPacketsProcessor.o: src/tcpGotFINPacketsProcessor.c src/tcpGotFINPacketsProcessor.h src/TCPHeaderData.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) -c -o bin/tcpGotFINPacketsProcessor.o src/tcpGotFINPacketsProcessor.c
bin/sendSiteQueueItems.o: src/sendSiteQueueItems.c src/sendSiteQueueItems.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPSiteQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/sendSiteQueueItems.o src/sendSiteQueueItems.c
bin/avl.o: src/contrib/avl.c src/contrib/avl.h
	$(CC) $(CFLAGS) -c -o bin/avl.o src/contrib/avl.c
bin/heap.o: src/contrib/heap.c src/contrib/heap.h
	$(CC) $(CFLAGS) -c -o bin/heap.o src/contrib/heap.c
clean:
	rm -f bin/*

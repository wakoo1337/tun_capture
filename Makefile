LIBEVENT_LIBS := -levent -levent_pthreads
LIBEVENT_CFLAGS := 
AR := ar
CC := cc
CFLAGS := -O0 -g -Wall -std=gnu11
.PHONY: all
all: bin/tun_capture
bin/tun_capture: bin/tun_capture.o bin/openInterface.o bin/doCapture.o bin/tunReadCallback.o bin/set16Bit.o bin/set32Bit.o bin/savePacket.o bin/dequeueRxPacket.o bin/threadWorker.o bin/compareIPv4FragmentsIdsSources.o bin/compareTimeoutItems.o bin/processIPv4Packet.o bin/compareTimeval.o bin/get16Bit.o bin/get32Bit.o bin/parseIPv4Header.o bin/addShortWithCarry.o bin/udpReadCallback.o bin/udpWriteCallback.o bin/compareUDPBindings.o bin/processUDPPacket.o bin/setIPv4SockaddrPort.o bin/findUDPBinding.o bin/packetsProcessor.o bin/udpGenerator.o bin/computeIPv4FragmentsCount.o bin/ipv4_strategy.o bin/fillIPv4FragmentMetadatas.o bin/createIPv4PseudoHdr.o bin/writeIPv4Headers.o bin/getIPv4SockaddrPort.o bin/readPacket.o bin/writePacket.o bin/getFdFromUser.o bin/addTimeval.o bin/subTimeval.o bin/getMonotonicTimeval.o bin/timerCallback.o bin/startTimer.o bin/parseTCPHeader.o bin/processTCPPacket.o bin/compareTCPConnections.o bin/tcpReadCallback.o bin/tcpstate_connwait.o bin/tcpConnwaitReadCallback.o bin/tcpConnwaitWriteCallback.o bin/destroyTCPConnection.o bin/tcpConnwaitPacketsProcessor.o bin/writeTCPHeader.o bin/tcpSynackSendReadCallback.o bin/tcpSynackSendWriteCallback.o bin/tcpSynackSendPacketsProcessor.o bin/tcpstate_synack_send.o bin/getSendWindowSize.o bin/computeIPv4MSS.o bin/computeIPv6MSS.o bin/sendSynReply.o bin/retry_delay.o bin/enqueueTimeout.o bin/enqueueTCPPacketTransmission.o bin/tcpRetransmissionTimerCallback.o bin/computeTCPDataOffset.o bin/enqueueRxPacket.o bin/tcpstate_established.o bin/tcpEstablishedReadCallback.o bin/tcpEstablishedWriteCallback.o bin/tcpEstablishedPacketsProcessor.o bin/cancelTimeout.o bin/tcpCleanupConfirmed.o bin/tcpUpdateReadEvent.o bin/tcpUpdateWriteEvent.o bin/checkByteInWindow.o bin/compareTCPSitePrequeueItems.o bin/segexpire_delay.o bin/tcpDeleteExpiredSegment.o bin/prequeueItemToSiteData.o bin/sendTCPAcknowledgement.o bin/processTCPUrgentData.o bin/processTCPData.o bin/dequeueTxPacket.o bin/tunWriteCallback.o bin/emergencyStop.o bin/enqueueTxPacket.o bin/tcpWriteCallback.o bin/tcpDestroySitePrequeueItem.o bin/tcpReadFinalizer.o bin/tcpWriteFinalizer.o bin/tcpstate_gotfin.o bin/sendPacketOnce.o bin/sendTCPPacketRefcounted.o bin/tcpGotFINReadCallback.o bin/tcpGotFINWriteCallback.o bin/tcpGotFINPacketsProcessor.o bin/sendSiteQueueItems.o bin/tcpEstablishedOnEnd.o bin/tcpEstablishedOnError.o bin/readAndEnqueueSiteData.o bin/tcpGotFINOnEnd.o bin/tcpGotFINOnError.o bin/isNewAckAcceptable.o bin/addressFamilyToNetworkStrategy.o bin/sendTCPFinalize.o bin/scaleRemoteWindow.o bin/tcpLastACKWaitReadCallback.o bin/tcpLastACKWaitWriteCallback.o bin/tcpLastACKWaitPacketsProcessor.o bin/tcpstate_lastackwait.o bin/enqueueTCPRetransmission.o bin/isAppQueueItemInWindow.o bin/enqueueUnsentTCPPacketsTransmission.o bin/udpFinalize.o bin/cancelTimeoutUnlocked.o bin/freeNoRefsAppQueueItem.o bin/findPreviousNextAppQueueItem.o bin/incrementAppQueueItemRefCount.o bin/decrementAppQueueItemRefCount.o bin/tcpFinalizeRead.o bin/tcpFinalizeWrite.o bin/tcpstate_connreset.o bin/tcpConnResetReadCallback.o bin/tcpConnResetWriteCallback.o bin/tcpConnResetPacketsProcessor.o bin/tcpstate_recvzero.o bin/tcpRecvZeroReadCallback.o bin/tcpRecvZeroWriteCallback.o bin/tcpRecvZeroPacketsProcessor.o bin/checkPacketInRecieveWindow.o bin/addPacketToPrequeue.o bin/prequeueToSiteQueue.o bin/tcpEstablishedOnFIN.o bin/timewait_delay.o bin/tcpRecvZeroOnFIN.o bin/tcpTimeWaitExpiredCallback.o bin/tcpstate_timewait.o bin/tcpTimeWaitReadCallback.o bin/tcpTimeWaitWriteCallback.o bin/tcpTimeWaitPacketsProcessor.o bin/enqueueStubTCPPacketQueueItem.o bin/tcpstate_lastsend.o bin/tcpLastSendReadCallback.o bin/tcpLastSendWriteCallback.o bin/tcpLastSendPacketsProcessor.o bin/checksum.a bin/avl.o bin/logdel_heap.o
	$(CC) $(CFLAGS) -o bin/tun_capture bin/tun_capture.o bin/openInterface.o bin/doCapture.o bin/tunReadCallback.o bin/set16Bit.o bin/set32Bit.o bin/savePacket.o bin/dequeueRxPacket.o bin/threadWorker.o bin/compareIPv4FragmentsIdsSources.o bin/compareTimeoutItems.o bin/compareTimeval.o bin/processIPv4Packet.o bin/get16Bit.o bin/get32Bit.o bin/parseIPv4Header.o bin/addShortWithCarry.o bin/udpReadCallback.o bin/udpWriteCallback.o bin/compareUDPBindings.o bin/processUDPPacket.o bin/setIPv4SockaddrPort.o bin/findUDPBinding.o bin/packetsProcessor.o bin/udpGenerator.o bin/computeIPv4FragmentsCount.o bin/ipv4_strategy.o bin/fillIPv4FragmentMetadatas.o bin/createIPv4PseudoHdr.o bin/writeIPv4Headers.o bin/getIPv4SockaddrPort.o bin/readPacket.o bin/writePacket.o bin/getFdFromUser.o bin/addTimeval.o bin/subTimeval.o bin/getMonotonicTimeval.o bin/timerCallback.o bin/startTimer.o bin/parseTCPHeader.o bin/processTCPPacket.o bin/compareTCPConnections.o bin/tcpReadCallback.o bin/tcpstate_connwait.o bin/tcpConnwaitReadCallback.o bin/tcpConnwaitWriteCallback.o bin/destroyTCPConnection.o bin/tcpConnwaitPacketsProcessor.o bin/writeTCPHeader.o bin/tcpSynackSendReadCallback.o bin/tcpSynackSendWriteCallback.o bin/tcpSynackSendPacketsProcessor.o bin/tcpstate_synack_send.o bin/getSendWindowSize.o bin/computeIPv4MSS.o bin/computeIPv6MSS.o bin/sendSynReply.o bin/retry_delay.o bin/enqueueTimeout.o bin/enqueueTCPPacketTransmission.o bin/tcpRetransmissionTimerCallback.o bin/computeTCPDataOffset.o bin/enqueueRxPacket.o bin/tcpstate_established.o bin/tcpEstablishedReadCallback.o bin/tcpEstablishedWriteCallback.o bin/tcpEstablishedPacketsProcessor.o bin/cancelTimeout.o bin/tcpCleanupConfirmed.o bin/tcpUpdateReadEvent.o bin/tcpUpdateWriteEvent.o bin/checkByteInWindow.o bin/compareTCPSitePrequeueItems.o bin/segexpire_delay.o bin/tcpDeleteExpiredSegment.o bin/prequeueItemToSiteData.o bin/sendTCPAcknowledgement.o bin/processTCPUrgentData.o bin/processTCPData.o bin/dequeueTxPacket.o bin/tunWriteCallback.o bin/emergencyStop.o bin/enqueueTxPacket.o bin/tcpWriteCallback.o bin/tcpDestroySitePrequeueItem.o bin/tcpReadFinalizer.o bin/tcpWriteFinalizer.o bin/tcpstate_gotfin.o bin/sendPacketOnce.o bin/sendTCPPacketRefcounted.o bin/tcpGotFINReadCallback.o bin/tcpGotFINWriteCallback.o bin/tcpGotFINPacketsProcessor.o bin/sendSiteQueueItems.o bin/tcpEstablishedOnEnd.o bin/tcpEstablishedOnError.o bin/readAndEnqueueSiteData.o bin/tcpGotFINOnEnd.o bin/tcpGotFINOnError.o bin/isNewAckAcceptable.o bin/addressFamilyToNetworkStrategy.o bin/sendTCPFinalize.o bin/scaleRemoteWindow.o bin/tcpLastACKWaitReadCallback.o bin/tcpLastACKWaitWriteCallback.o bin/tcpLastACKWaitPacketsProcessor.o bin/tcpstate_lastackwait.o bin/enqueueTCPRetransmission.o bin/isAppQueueItemInWindow.o bin/enqueueUnsentTCPPacketsTransmission.o bin/udpFinalize.o bin/cancelTimeoutUnlocked.o bin/freeNoRefsAppQueueItem.o bin/findPreviousNextAppQueueItem.o bin/incrementAppQueueItemRefCount.o bin/decrementAppQueueItemRefCount.o bin/tcpFinalizeRead.o bin/tcpFinalizeWrite.o bin/tcpstate_connreset.o bin/tcpConnResetReadCallback.o bin/tcpConnResetWriteCallback.o bin/tcpConnResetPacketsProcessor.o bin/tcpstate_recvzero.o bin/tcpRecvZeroReadCallback.o bin/tcpRecvZeroWriteCallback.o bin/tcpRecvZeroPacketsProcessor.o bin/checkPacketInRecieveWindow.o bin/addPacketToPrequeue.o bin/prequeueToSiteQueue.o bin/tcpEstablishedOnFIN.o bin/timewait_delay.o bin/tcpRecvZeroOnFIN.o bin/tcpTimeWaitExpiredCallback.o bin/tcpstate_timewait.o bin/tcpTimeWaitReadCallback.o bin/tcpTimeWaitWriteCallback.o bin/tcpTimeWaitPacketsProcessor.o bin/enqueueStubTCPPacketQueueItem.o bin/tcpstate_lastsend.o bin/tcpLastSendReadCallback.o bin/tcpLastSendWriteCallback.o bin/tcpLastSendPacketsProcessor.o bin/checksum.a bin/avl.o bin/logdel_heap.o $(LIBEVENT_LIBS) -lpthread
bin/tun_capture.o: src/tun_capture.c src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/UserData.h src/CaptureSettings.h src/openInterface.h src/doCapture.h src/set32Bit.h src/set16Bit.h src/getFdFromUser.h src/readPacket.h src/writePacket.h
	$(CC) $(CFLAGS) -c -o bin/tun_capture.o src/tun_capture.c
bin/openInterface.o: src/openInterface.c src/openInterface.h
	$(CC) $(CFLAGS) -c -o bin/openInterface.o src/openInterface.c
bin/doCapture.o: src/doCapture.c src/doCapture.h src/contrib/avl.h src/contrib/logdel_heap.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureContext.h src/CaptureSettings.h src/UDPBinding.h src/tunReadCallback.h src/tunWriteCallback.h src/threadWorker.h src/compareIPv4FragmentsIdsSources.h src/compareTimeoutItems.h src/compareUDPBindings.h src/timerCallback.h src/compareTCPConnections.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/doCapture.o src/doCapture.c
bin/tunReadCallback.o: src/tunReadCallback.c src/tunReadCallback.h src/CaptureSettings.h src/CaptureContext.h src/PacketQueueItem.h src/enqueueRxPacket.h src/packetsProcessor.h src/emergencyStop.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tunReadCallback.o src/tunReadCallback.c
bin/set16Bit.o: src/set16Bit.c src/set16Bit.h
	$(CC) $(CFLAGS) -c -o bin/set16Bit.o src/set16Bit.c
bin/set32Bit.o: src/set32Bit.c src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/set32Bit.o src/set32Bit.c
bin/savePacket.o: src/savePacket.c src/savePacket.h src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/savePacket.o src/savePacket.c
bin/dequeueRxPacket.o: src/dequeueRxPacket.c src/dequeueRxPacket.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/dequeueRxPacket.o src/dequeueRxPacket.c
bin/threadWorker.o: src/threadWorker.c src/threadWorker.h src/CaptureContext.h src/PacketQueueItem.h src/dequeueRxPacket.h src/emergencyStop.h
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
bin/processIPv4Packet.o: src/processIPv4Packet.c src/processIPv4Packet.h src/CaptureContext.h src/IPv4HeaderData.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/parseIPv4Header.h src/processUDPPacket.h src/ipv4_strategy.h src/processTCPPacket.h
	$(CC) $(CFLAGS) -c -o bin/processIPv4Packet.o src/processIPv4Packet.c
bin/parseIPv4Header.o: src/parseIPv4Header.c src/parseIPv4Header.h src/IPv4HeaderData.h src/ChecksumContext.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/get16Bit.h src/get32Bit.h src/set16Bit.h
	$(CC) $(CFLAGS) -c -o bin/parseIPv4Header.o src/parseIPv4Header.c
bin/get16Bit.o: src/get16Bit.c src/get16Bit.h
	$(CC) $(CFLAGS) -c -o bin/get16Bit.o src/get16Bit.c
bin/get32Bit.o: src/get32Bit.c src/get32Bit.h
	$(CC) $(CFLAGS) -c -o bin/get32Bit.o src/get32Bit.c
bin/addShortWithCarry.o: src/addShortWithCarry.c src/addShortWithCarry.h
	$(CC) $(CFLAGS) -c -o bin/addShortWithCarry.o src/addShortWithCarry.c
bin/udpReadCallback.o: src/udpReadCallback.c src/udpReadCallback.h src/CaptureContext.h src/PacketQueueItem.h src/UDPParameters.h src/UDPStackItem.h src/UDPBinding.h src/udpGenerator.h src/emergencyStop.h src/enqueueRxPacket.h src/HEADERS_RESERVE.h src/MAX_UDP_PAYLOAD.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/udpReadCallback.o src/udpReadCallback.c
bin/compareUDPBindings.o: src/compareUDPBindings.c src/compareUDPBindings.h src/UDPBinding.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/compareUDPBindings.o src/compareUDPBindings.c
bin/processUDPPacket.o: src/processUDPPacket.c src/processUDPPacket.h src/CaptureContext.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/UDPHeaderData.h src/UDPStackItem.h src/ChecksumContext.h src/UDPBinding.h src/get16Bit.h src/set16Bit.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/findUDPBinding.h src/udpReadCallback.h src/udpWriteCallback.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/processUDPPacket.o src/processUDPPacket.c
bin/setIPv4SockaddrPort.o: src/setIPv4SockaddrPort.c src/setIPv4SockaddrPort.h
	$(CC) $(CFLAGS) -c -o bin/setIPv4SockaddrPort.o src/setIPv4SockaddrPort.c
bin/findUDPBinding.o: src/findUDPBinding.c src/findUDPBinding.h src/contrib/avl.h src/CaptureContext.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/UDPBinding.h src/udpReadCallback.h src/udpWriteCallback.h src/udpFinalize.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/findUDPBinding.o src/findUDPBinding.c
bin/packetsProcessor.o: src/packetsProcessor.c src/packetsProcessor.h src/CaptureContext.h src/processIPv4Packet.h
	$(CC) $(CFLAGS) -c -o bin/packetsProcessor.o src/packetsProcessor.c
bin/udpGenerator.o: src/udpGenerator.c src/udpGenerator.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureSettings.h src/CaptureContext.h src/UDPParameters.h src/UDPBinding.h src/ChecksumContext.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/PacketQueueItem.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/set16Bit.h src/enqueueTxPacket.h src/sendPacketOnce.h src/addressFamilyToNetworkStrategy.h src/HEADERS_RESERVE.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/udpGenerator.o src/udpGenerator.c
bin/computeIPv4FragmentsCount.o: src/computeIPv4FragmentsCount.c src/computeIPv4FragmentsCount.h
	$(CC) $(CFLAGS) -c -o bin/computeIPv4FragmentsCount.o src/computeIPv4FragmentsCount.c
bin/ipv4_strategy.o: src/ipv4_strategy.c src/ipv4_strategy.h src/CaptureContext.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/setIPv4SockaddrPort.h src/getIPv4SockaddrPort.h src/computeIPv4FragmentsCount.h src/fillIPv4FragmentMetadatas.h src/createIPv4PseudoHdr.h src/writeIPv4Headers.h src/computeIPv4MSS.h
	$(CC) $(CFLAGS) -c -o bin/ipv4_strategy.o src/ipv4_strategy.c
bin/fillIPv4FragmentMetadatas.o: src/fillIPv4FragmentMetadatas.c src/fillIPv4FragmentMetadatas.h src/IPFragmentMetadata.h
	$(CC) $(CFLAGS) -c -o bin/fillIPv4FragmentMetadatas.o src/fillIPv4FragmentMetadatas.c
bin/createIPv4PseudoHdr.o: src/createIPv4PseudoHdr.c src/createIPv4PseudoHdr.h
	$(CC) $(CFLAGS) -c -o bin/createIPv4PseudoHdr.o src/createIPv4PseudoHdr.c
bin/writeIPv4Headers.o: src/writeIPv4Headers.c src/writeIPv4Headers.h src/ForwardingMappingIPv4.h src/ForwardingMappingIPv6.h src/CaptureSettings.h src/CaptureContext.h src/IPFragmentMetadata.h src/ChecksumContext.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/set16Bit.h
	$(CC) $(CFLAGS) -c -o bin/writeIPv4Headers.o src/writeIPv4Headers.c
bin/getIPv4SockaddrPort.o: src/getIPv4SockaddrPort.c src/getIPv4SockaddrPort.h
	$(CC) $(CFLAGS) -c -o bin/getIPv4SockaddrPort.o src/getIPv4SockaddrPort.c
bin/readPacket.o: src/readPacket.c src/readPacket.h src/UserData.h src/savePacket.h
	$(CC) $(CFLAGS) -c -o bin/readPacket.o src/readPacket.c
bin/writePacket.o: src/writePacket.c src/writePacket.h src/UserData.h src/savePacket.h
	$(CC) $(CFLAGS) -c -o bin/writePacket.o src/writePacket.c
bin/getFdFromUser.o: src/getFdFromUser.c src/getFdFromUser.h src/UserData.h
	$(CC) $(CFLAGS) -c -o bin/getFdFromUser.o src/getFdFromUser.c
bin/timerCallback.o: src/timerCallback.c src/timerCallback.h src/contrib/logdel_heap.h src/TimeoutItem.h src/CaptureContext.h src/getMonotonicTimeval.h src/compareTimeval.h src/startTimer.h src/emergencyStop.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/timerCallback.o src/timerCallback.c
bin/addTimeval.o: src/addTimeval.c src/addTimeval.h
	$(CC) $(CFLAGS) -c -o bin/addTimeval.o src/addTimeval.c
bin/subTimeval.o: src/subTimeval.c src/subTimeval.h
	$(CC) $(CFLAGS) -c -o bin/subTimeval.o src/subTimeval.c
bin/getMonotonicTimeval.o: src/getMonotonicTimeval.c src/getMonotonicTimeval.h
	$(CC) $(CFLAGS) -c -o bin/getMonotonicTimeval.o src/getMonotonicTimeval.c
bin/startTimer.o: src/startTimer.c src/startTimer.h src/contrib/logdel_heap.h src/TimeoutItem.h src/CaptureContext.h src/getMonotonicTimeval.h src/subTimeval.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/startTimer.o src/startTimer.c
bin/parseTCPHeader.o: src/parseTCPHeader.c src/parseTCPHeader.h src/TCPHeaderData.h src/ChecksumContext.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/get16Bit.h src/get32Bit.h
	$(CC) $(CFLAGS) -c -o bin/parseTCPHeader.o src/parseTCPHeader.c
bin/processTCPPacket.o: src/processTCPPacket.c src/processTCPPacket.h src/contrib/avl.h src/CaptureSettings.h src/CaptureContext.h src/IPPacketPayload.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/SrcDstSockaddrs.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/parseTCPHeader.h src/compareTCPSitePrequeueItems.h src/tcpReadCallback.h src/tcpWriteCallback.h src/tcpDestroySitePrequeueItem.h src/tcpFinalizeRead.h src/tcpFinalizeWrite.h src/tcpstate_connwait.h src/tcpstate_connreset.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/processTCPPacket.o src/processTCPPacket.c
bin/compareTCPConnections.o: src/compareTCPConnections.c src/compareTCPConnections.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) -c -o bin/compareTCPConnections.o src/compareTCPConnections.c
bin/tcpReadCallback.o: src/tcpReadCallback.c src/tcpReadCallback.h src/CaptureContext.h src/SrcDstSockaddrs.h src/IPPacketPayload.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/emergencyStop.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpReadCallback.o src/tcpReadCallback.c
bin/tcpstate_connwait.o: src/tcpstate_connwait.c src/tcpstate_connwait.h src/SrcDstSockaddrs.h src/IPPacketPayload.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/tcpConnwaitReadCallback.h src/tcpConnwaitWriteCallback.h src/tcpConnwaitPacketsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_connwait.o src/tcpstate_connwait.c
bin/tcpConnwaitReadCallback.o: src/tcpConnwaitReadCallback.c src/tcpConnwaitReadCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpConnwaitReadCallback.o src/tcpConnwaitReadCallback.c
bin/tcpConnwaitWriteCallback.o: src/tcpConnwaitWriteCallback.c src/tcpConnwaitWriteCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpConnwaitWriteCallback.o src/tcpConnwaitWriteCallback.c
bin/destroyTCPConnection.o: src/destroyTCPConnection.c src/destroyTCPConnection.h src/contrib/avl.h src/SrcDstSockaddrs.h src/CaptureSettings.h src/CaptureContext.h src/TCPConnection.h src/TCPAppQueueItem.h src/TCPSiteQueueItem.h src/PacketQueueItem.h src/tcpDestroySitePrequeueItem.h src/cancelTimeout.h src/freeNoRefsAppQueueItem.h src/decrementAppQueueItemRefCount.h src/sendTCPPacketRefcounted.h src/processTCPData.h
	$(CC) $(CFLAGS) -c -o bin/destroyTCPConnection.o src/destroyTCPConnection.c
bin/tcpConnwaitPacketsProcessor.o: src/tcpConnwaitPacketsProcessor.c src/tcpConnwaitPacketsProcessor.h src/IPPacketPayload.h src/TCPHeaderData.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) -c -o bin/tcpConnwaitPacketsProcessor.o src/tcpConnwaitPacketsProcessor.c
bin/writeTCPHeader.o: src/writeTCPHeader.c src/writeTCPHeader.h src/ChecksumContext.h src/TCPHeaderData.h src/initChecksum.h src/computeChecksum.h src/getChecksum.h src/set16Bit.h src/set32Bit.h
	$(CC) $(CFLAGS) -c -o bin/writeTCPHeader.o src/writeTCPHeader.c
bin/tcpSynackSendReadCallback.o: src/tcpSynackSendReadCallback.c src/tcpSynackSendReadCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpSynackSendReadCallback.o src/tcpSynackSendReadCallback.c
bin/tcpSynackSendWriteCallback.o: src/tcpSynackSendWriteCallback.c src/tcpSynackSendWriteCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpSynackSendWriteCallback.o src/tcpSynackSendWriteCallback.c
bin/tcpSynackSendPacketsProcessor.o: src/tcpSynackSendPacketsProcessor.c src/tcpSynackSendPacketsProcessor.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPHeaderData.h src/TimeoutItem.h src/tcpCleanupConfirmed.h src/tcpUpdateReadEvent.h src/tcpUpdateWriteEvent.h src/scaleRemoteWindow.h src/tcpstate_established.h
	$(CC) $(CFLAGS) -c -o bin/tcpSynackSendPacketsProcessor.o src/tcpSynackSendPacketsProcessor.c
bin/tcpstate_synack_send.o: src/tcpstate_synack_send.c src/tcpstate_synack_send.h src/SrcDstSockaddrs.h src/IPPacketPayload.h src/TCPConnection.h src/TCPHeaderData.h src/TCPState.h src/tcpSynackSendReadCallback.h src/tcpSynackSendWriteCallback.h src/tcpSynackSendPacketsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_synack_send.o src/tcpstate_synack_send.c
bin/getSendWindowSize.o: src/getSendWindowSize.c src/getSendWindowSize.h src/SrcDstSockaddrs.h src/TCPSiteQueueItem.h src/TCPConnection.h src/MAX_SITE_QUEUE.h
	$(CC) $(CFLAGS) -c -o bin/getSendWindowSize.o src/getSendWindowSize.c
bin/computeIPv4MSS.o: src/computeIPv4MSS.c src/computeIPv4MSS.h src/CaptureSettings.h src/CaptureContext.h
	$(CC) $(CFLAGS) -c -o bin/computeIPv4MSS.o src/computeIPv4MSS.c
bin/computeIPv6MSS.o: src/computeIPv6MSS.c src/computeIPv6MSS.h src/CaptureSettings.h src/CaptureContext.h
	$(CC) $(CFLAGS) -c -o bin/computeIPv6MSS.o src/computeIPv6MSS.c
bin/sendSynReply.o: src/sendSynReply.c src/sendSynReply.h src/CaptureSettings.h src/CaptureContext.h src/SrcDstSockaddrs.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/TCPAppQueueItem.h src/TCPHeaderData.h src/TCPConnection.h src/getSendWindowSize.h src/writeTCPHeader.h src/enqueueTCPPacketTransmission.h src/computeTCPDataOffset.h src/enqueueTCPRetransmission.h src/decrementAppQueueItemRefCount.h src/freeNoRefsAppQueueItem.h src/HEADERS_RESERVE.h
	$(CC) $(CFLAGS) -c -o bin/sendSynReply.o src/sendSynReply.c
bin/retry_delay.o: src/retry_delay.c src/retry_delay.h
	$(CC) $(CFLAGS) -c -o bin/retry_delay.o src/retry_delay.c
bin/enqueueTimeout.o: src/enqueueTimeout.c src/enqueueTimeout.h src/contrib/logdel_heap.h src/CaptureContext.h src/TimeoutItem.h
	$(CC) $(CFLAGS) -c -o bin/enqueueTimeout.o src/enqueueTimeout.c
bin/enqueueTCPPacketTransmission.o: src/enqueueTCPPacketTransmission.c src/enqueueTCPPacketTransmission.h src/CaptureSettings.h src/CaptureContext.h src/PacketQueueItem.h src/TCPAppQueueItem.h src/SrcDstSockaddrs.h src/TCPConnection.h src/enqueueTxPacket.h src/sendTCPPacketRefcounted.h src/incrementAppQueueItemRefCount.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/enqueueTCPPacketTransmission.o src/enqueueTCPPacketTransmission.c
bin/tcpRetransmissionTimerCallback.o: src/tcpRetransmissionTimerCallback.c src/tcpRetransmissionTimerCallback.h src/TCPAppQueueItem.h src/enqueueTCPPacketTransmission.h src/enqueueTCPRetransmission.h src/incrementAppQueueItemRefCount.h src/decrementAppQueueItemRefCount.h src/freeNoRefsAppQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/tcpRetransmissionTimerCallback.o src/tcpRetransmissionTimerCallback.c
bin/computeTCPDataOffset.o: src/computeTCPDataOffset.c src/computeTCPDataOffset.h src/TCPHeaderData.h
	$(CC) $(CFLAGS) -c -o bin/computeTCPDataOffset.o src/computeTCPDataOffset.c
bin/enqueueRxPacket.o: src/enqueueRxPacket.c src/enqueueRxPacket.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/enqueueRxPacket.o src/enqueueRxPacket.c
bin/tcpstate_established.o: src/tcpstate_established.c src/tcpstate_established.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/tcpEstablishedReadCallback.h src/tcpEstablishedWriteCallback.h src/tcpEstablishedPacketsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_established.o src/tcpstate_established.c
bin/tcpEstablishedReadCallback.o: src/tcpEstablishedReadCallback.c src/tcpEstablishedReadCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h src/readAndEnqueueSiteData.h src/tcpEstablishedOnEnd.h src/tcpEstablishedOnError.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpEstablishedReadCallback.o src/tcpEstablishedReadCallback.c
bin/tcpEstablishedWriteCallback.o: src/tcpEstablishedWriteCallback.c src/tcpEstablishedWriteCallback.h src/SrcDstSockaddrs.h src/TCPSiteQueueItem.h src/TCPConnection.h src/getSendWindowSize.h src/sendTCPAcknowledgement.h src/sendSiteQueueItems.h src/tcpUpdateWriteEvent.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpEstablishedWriteCallback.o src/tcpEstablishedWriteCallback.c
bin/tcpEstablishedPacketsProcessor.o: src/tcpEstablishedPacketsProcessor.c src/tcpEstablishedPacketsProcessor.h src/contrib/avl.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPHeaderData.h src/TCPSitePrequeueItem.h src/prequeueItemToSiteData.h src/tcpUpdateReadEvent.h src/tcpUpdateWriteEvent.h src/sendTCPAcknowledgement.h src/tcpCleanupConfirmed.h src/isNewAckAcceptable.h src/scaleRemoteWindow.h src/enqueueUnsentTCPPacketsTransmission.h src/checkPacketInRecieveWindow.h src/tcpEstablishedOnFIN.h src/addPacketToPrequeue.h src/prequeueToSiteQueue.h src/tcpstate_established.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpEstablishedPacketsProcessor.o src/tcpEstablishedPacketsProcessor.c
bin/cancelTimeout.o: src/cancelTimeout.c src/cancelTimeout.h src/CaptureContext.h src/TimeoutItem.h src/cancelTimeoutUnlocked.h
	$(CC) $(CFLAGS) -c -o bin/cancelTimeout.o src/cancelTimeout.c
bin/tcpCleanupConfirmed.o: src/tcpCleanupConfirmed.c src/tcpCleanupConfirmed.h src/SrcDstSockaddrs.h src/CaptureContext.h src/TCPAppQueueItem.h src/TCPConnection.h src/cancelTimeout.h src/freeNoRefsAppQueueItem.h src/incrementAppQueueItemRefCount.h src/decrementAppQueueItemRefCount.h
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
bin/tcpDeleteExpiredSegment.o: src/tcpDeleteExpiredSegment.c src/tcpDeleteExpiredSegment.h src/contrib/avl.h src/TimeoutItem.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPSitePrequeueItem.h
	$(CC) $(CFLAGS) -c -o bin/tcpDeleteExpiredSegment.o src/tcpDeleteExpiredSegment.c
bin/prequeueItemToSiteData.o: src/prequeueItemToSiteData.c src/prequeueItemToSiteData.h src/contrib/avl.h src/SrcDstSockaddrs.h src/TCPSitePrequeueItem.h src/TCPSiteQueueItem.h src/TCPConnection.h
	$(CC) $(CFLAGS) -c -o bin/prequeueItemToSiteData.o src/prequeueItemToSiteData.c
bin/sendTCPAcknowledgement.o: src/sendTCPAcknowledgement.c src/sendTCPAcknowledgement.h src/CaptureSettings.h src/CaptureContext.h src/TCPHeaderData.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPAppQueueItem.h src/writeTCPHeader.h src/enqueueTCPPacketTransmission.h src/getSendWindowSize.h src/computeTCPDataOffset.h src/HEADERS_RESERVE.h
	$(CC) $(CFLAGS) -c -o bin/sendTCPAcknowledgement.o src/sendTCPAcknowledgement.c
bin/processTCPUrgentData.o: src/processTCPUrgentData.c src/processTCPUrgentData.h src/CaptureContext.h
	$(CC) $(CFLAGS) -c -o bin/processTCPUrgentData.o src/processTCPUrgentData.c
bin/processTCPData.o: src/processTCPData.c src/processTCPData.h src/CaptureSettings.h src/CaptureContext.h src/SrcDstSockaddrs.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/TCPConnection.h src/TCPAppQueueItem.h src/TCPHeaderData.h src/getSendWindowSize.h src/computeTCPDataOffset.h src/writeTCPHeader.h src/checkByteInWindow.h src/enqueueTCPPacketTransmission.h src/enqueueTCPRetransmission.h src/isAppQueueItemInWindow.h src/freeNoRefsAppQueueItem.h src/findPreviousNextAppQueueItem.h src/decrementAppQueueItemRefCount.h src/sendTCPFinalize.h src/HEADERS_RESERVE.h
	$(CC) $(CFLAGS) -c -o bin/processTCPData.o src/processTCPData.c
bin/dequeueTxPacket.o: src/dequeueTxPacket.c src/dequeueTxPacket.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/dequeueTxPacket.o src/dequeueTxPacket.c
bin/tunWriteCallback.o: src/tunWriteCallback.c src/tunWriteCallback.h src/CaptureSettings.h src/CaptureContext.h src/PacketQueueItem.h src/dequeueTxPacket.h src/emergencyStop.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tunWriteCallback.o src/tunWriteCallback.c
bin/emergencyStop.o: src/emergencyStop.c src/emergencyStop.h src/CaptureContext.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/emergencyStop.o src/emergencyStop.c
bin/udpWriteCallback.o: src/udpWriteCallback.c src/udpWriteCallback.h src/CaptureContext.h src/UDPBinding.h src/UDPStackItem.h src/emergencyStop.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/udpWriteCallback.o src/udpWriteCallback.c
bin/enqueueTxPacket.o: src/enqueueTxPacket.c src/enqueueTxPacket.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/enqueueTxPacket.o src/enqueueTxPacket.c
bin/tcpWriteCallback.o: src/tcpWriteCallback.c src/tcpWriteCallback.h src/CaptureContext.h src/SrcDstSockaddrs.h src/IPPacketPayload.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/emergencyStop.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpWriteCallback.o src/tcpWriteCallback.c
bin/tcpDestroySitePrequeueItem.o: src/tcpDestroySitePrequeueItem.c src/tcpDestroySitePrequeueItem.h src/SrcDstSockaddrs.h src/CaptureContext.h src/TCPConnection.h src/TCPSitePrequeueItem.h src/cancelTimeout.h
	$(CC) $(CFLAGS) -c -o bin/tcpDestroySitePrequeueItem.o src/tcpDestroySitePrequeueItem.c
bin/tcpReadFinalizer.o: src/tcpReadFinalizer.c src/tcpReadFinalizer.h src/SrcDstSockaddrs.h src/TCPConnection.h src/destroyTCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpReadFinalizer.o src/tcpReadFinalizer.c
bin/tcpWriteFinalizer.o: src/tcpWriteFinalizer.c src/tcpWriteFinalizer.h src/SrcDstSockaddrs.h src/TCPConnection.h src/destroyTCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpWriteFinalizer.o src/tcpWriteFinalizer.c
bin/tcpstate_gotfin.o: src/tcpstate_gotfin.c src/tcpstate_gotfin.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/tcpGotFINReadCallback.h src/tcpGotFINWriteCallback.h src/tcpGotFINPacketsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_gotfin.o src/tcpstate_gotfin.c
bin/sendPacketOnce.o: src/sendPacketOnce.c src/sendPacketOnce.h src/CaptureSettings.h src/CaptureContext.h src/PacketQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/sendPacketOnce.o src/sendPacketOnce.c
bin/sendTCPPacketRefcounted.o: src/sendTCPPacketRefcounted.c src/sendTCPPacketRefcounted.h src/CaptureSettings.h src/CaptureContext.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPAppQueueItem.h src/freeNoRefsAppQueueItem.h src/incrementAppQueueItemRefCount.h src/decrementAppQueueItemRefCount.h
	$(CC) $(CFLAGS) -c -o bin/sendTCPPacketRefcounted.o src/sendTCPPacketRefcounted.c
bin/tcpGotFINReadCallback.o: src/tcpGotFINReadCallback.c src/tcpGotFINReadCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h src/readAndEnqueueSiteData.h src/tcpGotFINOnEnd.h src/tcpGotFINOnError.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpGotFINReadCallback.o src/tcpGotFINReadCallback.c
bin/tcpGotFINWriteCallback.o: src/tcpGotFINWriteCallback.c src/tcpGotFINWriteCallback.h src/TCPSiteQueueItem.h src/SrcDstSockaddrs.h src/TCPConnection.h src/sendSiteQueueItems.h src/tcpFinalizeWrite.h src/tcpUpdateWriteEvent.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpGotFINWriteCallback.o src/tcpGotFINWriteCallback.c
bin/tcpGotFINPacketsProcessor.o: src/tcpGotFINPacketsProcessor.c src/tcpGotFINPacketsProcessor.h src/TCPHeaderData.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/TCPConnection.h src/isNewAckAcceptable.h src/tcpCleanupConfirmed.h src/enqueueUnsentTCPPacketsTransmission.h src/scaleRemoteWindow.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpGotFINPacketsProcessor.o src/tcpGotFINPacketsProcessor.c
bin/sendSiteQueueItems.o: src/sendSiteQueueItems.c src/sendSiteQueueItems.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPSiteQueueItem.h src/tcpFinalizeRead.h src/tcpFinalizeWrite.h src/tcpstate_connreset.h
	$(CC) $(CFLAGS) -c -o bin/sendSiteQueueItems.o src/sendSiteQueueItems.c
bin/tcpEstablishedOnEnd.o: src/tcpEstablishedOnEnd.c src/tcpEstablishedOnEnd.h src/SrcDstSockaddrs.h src/TCPConnection.h src/tcpFinalizeRead.h src/enqueueStubTCPPacketQueueItem.h src/tcpstate_recvzero.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpEstablishedOnEnd.o src/tcpEstablishedOnEnd.c
bin/tcpEstablishedOnError.o: src/tcpEstablishedOnError.c src/tcpEstablishedOnError.h src/SrcDstSockaddrs.h src/TCPConnection.h src/tcpFinalizeRead.h src/tcpFinalizeWrite.h src/tcpstate_connreset.h
	$(CC) $(CFLAGS) -c -o bin/tcpEstablishedOnError.o src/tcpEstablishedOnError.c
bin/readAndEnqueueSiteData.o: src/readAndEnqueueSiteData.c src/readAndEnqueueSiteData.h src/SrcDstSockaddrs.h src/TCPConnection.h src/PacketQueueItem.h src/enqueueRxPacket.h src/tcpUpdateReadEvent.h src/processTCPData.h src/HEADERS_RESERVE.h src/MAX_APP_QUEUE.h
	$(CC) $(CFLAGS) -c -o bin/readAndEnqueueSiteData.o src/readAndEnqueueSiteData.c
bin/tcpGotFINOnEnd.o: src/tcpGotFINOnEnd.c src/tcpGotFINOnEnd.h src/SrcDstSockaddrs.h src/TCPConnection.h src/enqueueStubTCPPacketQueueItem.h src/tcpstate_lastackwait.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpGotFINOnEnd.o src/tcpGotFINOnEnd.c
bin/tcpGotFINOnError.o: src/tcpGotFINOnError.c src/tcpGotFINOnError.h src/SrcDstSockaddrs.h src/TCPConnection.h src/tcpFinalizeRead.h src/tcpFinalizeWrite.h src/tcpstate_connreset.h
	$(CC) $(CFLAGS) -c -o bin/tcpGotFINOnError.o src/tcpGotFINOnError.c
bin/isNewAckAcceptable.o: src/isNewAckAcceptable.c src/isNewAckAcceptable.h src/SrcDstSockaddrs.h src/TCPConnection.h src/checkByteInWindow.h
	$(CC) $(CFLAGS) -c -o bin/isNewAckAcceptable.o src/isNewAckAcceptable.c
bin/addressFamilyToNetworkStrategy.o: src/addressFamilyToNetworkStrategy.c src/addressFamilyToNetworkStrategy.h src/CaptureContext.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/ipv4_strategy.h src/ipv6_strategy.h
	$(CC) $(CFLAGS) -c -o bin/addressFamilyToNetworkStrategy.o src/addressFamilyToNetworkStrategy.c
bin/sendTCPFinalize.o: src/sendTCPFinalize.c src/sendTCPFinalize.h src/CaptureSettings.h src/CaptureContext.h src/TCPHeaderData.h src/IPFragmentMetadata.h src/NetworkProtocolStrategy.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPAppQueueItem.h src/writeTCPHeader.h src/enqueueTCPPacketTransmission.h src/getSendWindowSize.h src/computeTCPDataOffset.h src/enqueueTCPRetransmission.h src/freeNoRefsAppQueueItem.h src/decrementAppQueueItemRefCount.h src/findPreviousNextAppQueueItem.h src/HEADERS_RESERVE.h
	$(CC) $(CFLAGS) -c -o bin/sendTCPFinalize.o src/sendTCPFinalize.c
bin/scaleRemoteWindow.o: src/scaleRemoteWindow.c src/scaleRemoteWindow.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) -c -o bin/scaleRemoteWindow.o src/scaleRemoteWindow.c
bin/tcpLastACKWaitReadCallback.o: src/tcpLastACKWaitReadCallback.c src/tcpLastACKWaitReadCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpLastACKWaitReadCallback.o src/tcpLastACKWaitReadCallback.c
bin/tcpLastACKWaitWriteCallback.o: src/tcpLastACKWaitWriteCallback.c src/tcpLastACKWaitWriteCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpLastACKWaitWriteCallback.o src/tcpLastACKWaitWriteCallback.c
bin/tcpLastACKWaitPacketsProcessor.o: src/tcpLastACKWaitPacketsProcessor.c src/tcpLastACKWaitPacketsProcessor.h src/SrcDstSockaddrs.h src/TCPConnection.h src/IPPacketPayload.h src/TCPHeaderData.h src/tcpCleanupConfirmed.h src/tcpFinalizeRead.h src/isNewAckAcceptable.h src/enqueueUnsentTCPPacketsTransmission.h src/scaleRemoteWindow.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpLastACKWaitPacketsProcessor.o src/tcpLastACKWaitPacketsProcessor.c
bin/tcpstate_lastackwait.o: src/tcpstate_lastackwait.c src/tcpstate_lastackwait.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/tcpLastACKWaitReadCallback.h src/tcpLastACKWaitWriteCallback.h src/tcpLastACKWaitPacketsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_lastackwait.o src/tcpstate_lastackwait.c
bin/enqueueTCPRetransmission.o: src/enqueueTCPRetransmission.c src/enqueueTCPRetransmission.h src/CaptureContext.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPAppQueueItem.h src/TimeoutItem.h src/getMonotonicTimeval.h src/addTimeval.h src/enqueueTimeout.h src/tcpRetransmissionTimerCallback.h src/startTimer.h src/cancelTimeoutUnlocked.h src/retry_delay.h
	$(CC) $(CFLAGS) -c -o bin/enqueueTCPRetransmission.o src/enqueueTCPRetransmission.c
bin/isAppQueueItemInWindow.o: src/isAppQueueItemInWindow.c src/isAppQueueItemInWindow.h src/TCPAppQueueItem.h src/checkByteInWindow.h
	$(CC) $(CFLAGS) -c -o bin/isAppQueueItemInWindow.o src/isAppQueueItemInWindow.c
bin/enqueueUnsentTCPPacketsTransmission.o: src/enqueueUnsentTCPPacketsTransmission.c src/enqueueUnsentTCPPacketsTransmission.h src/SrcDstSockaddrs.h src/TCPAppQueueItem.h src/TCPConnection.h src/isAppQueueItemInWindow.h src/enqueueTCPPacketTransmission.h src/enqueueTCPRetransmission.h src/freeNoRefsAppQueueItem.h src/incrementAppQueueItemRefCount.h src/decrementAppQueueItemRefCount.h
	$(CC) $(CFLAGS) -c -o bin/enqueueUnsentTCPPacketsTransmission.o src/enqueueUnsentTCPPacketsTransmission.c
bin/udpFinalize.o: src/udpFinalize.c src/udpFinalize.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/udpFinalize.o src/udpFinalize.c
bin/cancelTimeoutUnlocked.o: src/cancelTimeoutUnlocked.c src/cancelTimeoutUnlocked.h src/contrib/logdel_heap.h src/CaptureContext.h src/TimeoutItem.h
	$(CC) $(CFLAGS) -c -o bin/cancelTimeoutUnlocked.o src/cancelTimeoutUnlocked.c
bin/freeNoRefsAppQueueItem.o: src/freeNoRefsAppQueueItem.c src/freeNoRefsAppQueueItem.h src/SrcDstSockaddrs.h src/CaptureContext.h src/TCPConnection.h src/TCPAppQueueItem.h src/cancelTimeout.h
	$(CC) $(CFLAGS) -c -o bin/freeNoRefsAppQueueItem.o src/freeNoRefsAppQueueItem.c
bin/findPreviousNextAppQueueItem.o: src/findPreviousNextAppQueueItem.c src/findPreviousNextAppQueueItem.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPAppQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/findPreviousNextAppQueueItem.o src/findPreviousNextAppQueueItem.c
bin/incrementAppQueueItemRefCount.o: src/incrementAppQueueItemRefCount.c src/incrementAppQueueItemRefCount.h src/TCPAppQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/incrementAppQueueItemRefCount.o src/incrementAppQueueItemRefCount.c
bin/decrementAppQueueItemRefCount.o: src/decrementAppQueueItemRefCount.c src/decrementAppQueueItemRefCount.h src/TCPAppQueueItem.h
	$(CC) $(CFLAGS) -c -o bin/decrementAppQueueItemRefCount.o src/decrementAppQueueItemRefCount.c
bin/tcpFinalizeRead.o: src/tcpFinalizeRead.c src/tcpFinalizeRead.h src/SrcDstSockaddrs.h src/TCPConnection.h src/tcpReadFinalizer.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpFinalizeRead.o src/tcpFinalizeRead.c
bin/tcpFinalizeWrite.o: src/tcpFinalizeWrite.c src/tcpFinalizeWrite.h src/SrcDstSockaddrs.h src/TCPConnection.h src/tcpWriteFinalizer.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpFinalizeWrite.o src/tcpFinalizeWrite.c
bin/tcpstate_connreset.o: src/tcpstate_connreset.c src/tcpstate_connreset.h src/IPPacketPayload.h src/SrcDstSockaddrs.h src/TCPHeaderData.h src/TCPConnection.h src/TCPState.h src/tcpConnResetReadCallback.h src/tcpConnResetWriteCallback.h src/tcpConnResetPacketsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_connreset.o src/tcpstate_connreset.c
bin/tcpConnResetReadCallback.o: src/tcpConnResetReadCallback.c src/tcpConnResetReadCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpConnResetReadCallback.o src/tcpConnResetReadCallback.c
bin/tcpConnResetWriteCallback.o: src/tcpConnResetWriteCallback.c src/tcpConnResetWriteCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpConnResetWriteCallback.o src/tcpConnResetWriteCallback.c
bin/tcpConnResetPacketsProcessor.o: src/tcpConnResetPacketsProcessor.c src/tcpConnResetPacketsProcessor.h src/IPPacketPayload.h src/TCPHeaderData.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) -c -o bin/tcpConnResetPacketsProcessor.o src/tcpConnResetPacketsProcessor.c
bin/tcpstate_recvzero.o: src/tcpstate_recvzero.c src/tcpstate_recvzero.h src/SrcDstSockaddrs.h src/TCPConnection.h src/IPPacketPayload.h src/TCPHeaderData.h src/TCPState.h src/tcpRecvZeroReadCallback.h src/tcpRecvZeroWriteCallback.h src/tcpRecvZeroPacketsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_recvzero.o src/tcpstate_recvzero.c
bin/tcpRecvZeroReadCallback.o: src/tcpRecvZeroReadCallback.c src/tcpRecvZeroReadCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpRecvZeroReadCallback.o src/tcpRecvZeroReadCallback.c
bin/tcpRecvZeroWriteCallback.o: src/tcpRecvZeroWriteCallback.c src/tcpRecvZeroWriteCallback.h src/SrcDstSockaddrs.h src/TCPSiteQueueItem.h src/TCPConnection.h src/getSendWindowSize.h src/sendTCPAcknowledgement.h src/sendSiteQueueItems.h src/tcpUpdateWriteEvent.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpRecvZeroWriteCallback.o src/tcpRecvZeroWriteCallback.c
bin/tcpRecvZeroPacketsProcessor.o: src/tcpRecvZeroPacketsProcessor.c src/tcpRecvZeroPacketsProcessor.h src/SrcDstSockaddrs.h src/TCPConnection.h src/IPPacketPayload.h src/TCPHeaderData.h src/checkPacketInRecieveWindow.h src/isNewAckAcceptable.h src/addPacketToPrequeue.h src/tcpCleanupConfirmed.h src/prequeueToSiteQueue.h src/enqueueUnsentTCPPacketsTransmission.h src/tcpUpdateWriteEvent.h src/sendTCPAcknowledgement.h src/tcpRecvZeroOnFIN.h src/scaleRemoteWindow.h src/tcpstate_recvzero.h
	$(CC) $(CFLAGS) -c -o bin/tcpRecvZeroPacketsProcessor.o src/tcpRecvZeroPacketsProcessor.c
bin/checkPacketInRecieveWindow.o: src/checkPacketInRecieveWindow.c src/checkPacketInRecieveWindow.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPHeaderData.h src/IPPacketPayload.h src/checkByteInWindow.h src/MAX_SITE_QUEUE.h
	$(CC) $(CFLAGS) -c -o bin/checkPacketInRecieveWindow.o src/checkPacketInRecieveWindow.c
bin/addPacketToPrequeue.o: src/addPacketToPrequeue.c src/addPacketToPrequeue.h src/contrib/avl.h src/CaptureContext.h src/SrcDstSockaddrs.h src/TCPConnection.h src/IPPacketPayload.h src/TCPHeaderData.h src/TCPSitePrequeueItem.h src/getMonotonicTimeval.h src/addTimeval.h src/enqueueTimeout.h src/cancelTimeout.h src/startTimer.h src/tcpDeleteExpiredSegment.h src/segexpire_delay.h
	$(CC) $(CFLAGS) -c -o bin/addPacketToPrequeue.o src/addPacketToPrequeue.c
bin/prequeueToSiteQueue.o: src/prequeueToSiteQueue.c src/prequeueToSiteQueue.h src/contrib/avl.h src/SrcDstSockaddrs.h src/CaptureContext.h src/TCPConnection.h src/TCPSitePrequeueItem.h src/TimeoutItem.h src/cancelTimeoutUnlocked.h src/prequeueItemToSiteData.h
	$(CC) $(CFLAGS) -c -o bin/prequeueToSiteQueue.o src/prequeueToSiteQueue.c
bin/tcpEstablishedOnFIN.o: src/tcpEstablishedOnFIN.c src/tcpEstablishedOnFIN.h src/SrcDstSockaddrs.h src/TCPHeaderData.h src/IPPacketPayload.h src/TCPConnection.h src/TCPState.h src/tcpstate_gotfin.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpEstablishedOnFIN.o src/tcpEstablishedOnFIN.c
bin/timewait_delay.o: src/timewait_delay.c src/timewait_delay.h
	$(CC) $(CFLAGS) -c -o bin/timewait_delay.o src/timewait_delay.c
bin/tcpRecvZeroOnFIN.o: src/tcpRecvZeroOnFIN.c src/tcpRecvZeroOnFIN.h src/SrcDstSockaddrs.h src/TCPConnection.h src/tcpstate_lastsend.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpRecvZeroOnFIN.o src/tcpRecvZeroOnFIN.c
bin/tcpTimeWaitExpiredCallback.o: src/tcpTimeWaitExpiredCallback.c src/tcpTimeWaitExpiredCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h src/tcpFinalizeRead.h src/tcpFinalizeWrite.h
	$(CC) $(CFLAGS) -c -o bin/tcpTimeWaitExpiredCallback.o src/tcpTimeWaitExpiredCallback.c
bin/tcpstate_timewait.o: src/tcpstate_timewait.c src/tcpstate_timewait.h src/SrcDstSockaddrs.h src/TCPConnection.h src/TCPHeaderData.h src/IPPacketPayload.h src/TCPState.h src/tcpTimeWaitReadCallback.h src/tcpTimeWaitWriteCallback.h src/tcpTimeWaitPacketsProcessor.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_timewait.o src/tcpstate_timewait.c
bin/tcpTimeWaitReadCallback.o: src/tcpTimeWaitReadCallback.c src/tcpTimeWaitReadCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpTimeWaitReadCallback.o src/tcpTimeWaitReadCallback.c
bin/tcpTimeWaitWriteCallback.o: src/tcpTimeWaitWriteCallback.c src/tcpTimeWaitWriteCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpTimeWaitWriteCallback.o src/tcpTimeWaitWriteCallback.c
bin/tcpTimeWaitPacketsProcessor.o: src/tcpTimeWaitPacketsProcessor.c src/tcpTimeWaitPacketsProcessor.h src/SrcDstSockaddrs.h src/TCPConnection.h src/IPPacketPayload.h src/TCPHeaderData.h
	$(CC) $(CFLAGS) -c -o bin/tcpTimeWaitPacketsProcessor.o src/tcpTimeWaitPacketsProcessor.c
bin/enqueueStubTCPPacketQueueItem.o: src/enqueueStubTCPPacketQueueItem.c src/enqueueStubTCPPacketQueueItem.h src/CaptureContext.h src/SrcDstSockaddrs.h src/TCPConnection.h src/PacketQueueItem.h src/processTCPData.h src/enqueueRxPacket.h src/HEADERS_RESERVE.h
	$(CC) $(CFLAGS) -c -o bin/enqueueStubTCPPacketQueueItem.o src/enqueueStubTCPPacketQueueItem.c
bin/tcpstate_lastsend.o: src/tcpstate_lastsend.c src/tcpstate_lastsend.h src/SrcDstSockaddrs.h src/TCPConnection.h src/IPPacketPayload.h src/TCPHeaderData.h src/TCPState.h src/tcpLastSendReadCallback.h src/tcpLastSendWriteCallback.h src/tcpLastSendPacketsProcessor.h src/tcpstate_lastsend.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpstate_lastsend.o src/tcpstate_lastsend.c
bin/tcpLastSendReadCallback.o: src/tcpLastSendReadCallback.c src/tcpLastSendReadCallback.h src/SrcDstSockaddrs.h src/TCPConnection.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpLastSendReadCallback.o src/tcpLastSendReadCallback.c
bin/tcpLastSendWriteCallback.o: src/tcpLastSendWriteCallback.c src/tcpLastSendWriteCallback.h src/SrcDstSockaddrs.h src/CaptureContext.h src/TCPConnection.h src/getSendWindowSize.h src/sendSiteQueueItems.h src/tcpUpdateWriteEvent.h src/sendTCPAcknowledgement.h src/getMonotonicTimeval.h src/addTimeval.h src/enqueueTimeout.h src/startTimer.h src/tcpTimeWaitExpiredCallback.h src/tcpstate_timewait.h src/timewait_delay.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpLastSendWriteCallback.o src/tcpLastSendWriteCallback.c
bin/tcpLastSendPacketsProcessor.o: src/tcpLastSendPacketsProcessor.c src/tcpLastSendPacketsProcessor.h src/SrcDstSockaddrs.h src/TCPConnection.h src/IPPacketPayload.h src/TCPHeaderData.h src/isNewAckAcceptable.h src/scaleRemoteWindow.h src/tcpCleanupConfirmed.h src/sendTCPAcknowledgement.h src/enqueueUnsentTCPPacketsTransmission.h
	$(CC) $(CFLAGS) $(LIBEVENT_CFLAGS) -c -o bin/tcpLastSendPacketsProcessor.o src/tcpLastSendPacketsProcessor.c
bin/avl.o: src/contrib/avl.c src/contrib/avl.h
	$(CC) $(CFLAGS) -c -o bin/avl.o src/contrib/avl.c
bin/logdel_heap.o: src/contrib/logdel_heap.c src/contrib/logdel_heap.h
	$(CC) $(CFLAGS) -c -o bin/logdel_heap.o src/contrib/logdel_heap.c
clean:
	rm -f bin/*

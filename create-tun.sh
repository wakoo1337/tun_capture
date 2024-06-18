#!/bin/bash
VETH_PREFIX=cap
NETNS_NAME=capture_ns
USER_OWNER=wakoo
GROUP_OWNER=wakoo
EXTERNAL_V4=192.168.100.1
INTERNAL_V4=192.168.100.10
EXTERNAL_V6=fe80::0101
INTERNAL_V6=fe80::0110

ip netns add $NETNS_NAME
ip link add ${VETH_PREFIX}_int netns $NETNS_NAME type veth peer name ${VETH_PREFIX}_ext

ip link set dev ${VETH_PREFIX}_ext up
ip -n $NETNS_NAME link set dev ${VETH_PREFIX}_int up

ip address add local ${EXTERNAL_V4} peer ${INTERNAL_V4} dev ${VETH_PREFIX}_ext scope global
ip address add local ${EXTERNAL_V6} peer ${INTERNAL_V6} dev ${VETH_PREFIX}_ext scope global

ip -n $NETNS_NAME address add local ${INTERNAL_V4} peer ${EXTERNAL_V4} dev ${VETH_PREFIX}_int scope global
ip -n $NETNS_NAME address add local ${INTERNAL_V6} peer ${EXTERNAL_V6} dev ${VETH_PREFIX}_int scope global
ip -n $NETNS_NAME route add default via ${EXTERNAL_V4} dev ${VETH_PREFIX}_int
ip -n $NETNS_NAME route add default via ${EXTERNAL_V6} dev ${VETH_PREFIX}_int

ethtool -K ${VETH_PREFIX}_ext tx-checksumming off
ethtool -K ${VETH_PREFIX}_ext rx-checksumming off
ip netns exec $NETNS_NAME ethtool -K ${VETH_PREFIX}_int tx-checksumming off
ip netns exec $NETNS_NAME ethtool -K ${VETH_PREFIX}_int rx-checksumming off

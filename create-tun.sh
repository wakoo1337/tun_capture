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
ip link add ${VETH_PREFIX}_int address 1e:3d:27:af:3a:7c type veth peer name ${VETH_PREFIX}_ext address da:b0:04:bd:04:50

ip link set dev ${VETH_PREFIX}_ext up
ip link set dev ${VETH_PREFIX}_int up

ethtool -K ${VETH_PREFIX}_ext tx-checksumming off
ethtool -K ${VETH_PREFIX}_ext rx-checksumming off
ethtool -K ${VETH_PREFIX}_int tx-checksumming off
ethtool -K ${VETH_PREFIX}_int rx-checksumming off

ip link set dev ${VETH_PREFIX}_int netns $NETNS_NAME
ip -n $NETNS_NAME link set dev ${VETH_PREFIX}_int up
ip -n $NETNS_NAME link set dev lo up

ip address add local ${EXTERNAL_V4} peer ${INTERNAL_V4} dev ${VETH_PREFIX}_ext scope global
ip address add local ${EXTERNAL_V6} peer ${INTERNAL_V6} dev ${VETH_PREFIX}_ext scope global

ip -n $NETNS_NAME address add local ${INTERNAL_V4} peer ${EXTERNAL_V4} dev ${VETH_PREFIX}_int scope global
ip -n $NETNS_NAME address add local ${INTERNAL_V6} peer ${EXTERNAL_V6} dev ${VETH_PREFIX}_int scope global
ip -n $NETNS_NAME route add default via ${EXTERNAL_V4} dev ${VETH_PREFIX}_int
ip -n $NETNS_NAME route add default via ${EXTERNAL_V6} dev ${VETH_PREFIX}_int

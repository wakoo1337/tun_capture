#!/bin/bash
VETH_PREFIX=cap
NETNS_NAME=capture_ns
USER_OWNER=wakoo
GROUP_OWNER=wakoo
EXTERNAL_V4=192.168.100.1
INTERNAL_V4=192.168.100.10

ip netns add $NETNS_NAME
ip link add ${VETH_PREFIX}_int netns $NETNS_NAME type veth peer name ${VETH_PREFIX}_ext

ip link set dev ${VETH_PREFIX}_ext up
ip -n $NETNS_NAME link set dev ${VETH_PREFIX}_int up

EXTERNAL_V6=$(ip -6 address show dev ${VETH_PREFIX}_ext | sed -e's/^.*inet6 \([^ ]*\)\/.*$/\1/;t;d')
INTERNAL_V6=$(ip -6 -n ${NETNS_NAME} address show dev ${VETH_PREFIX}_int | sed -e's/^.*inet6 \([^ ]*\)\/.*$/\1/;t;d')

ip address add local ${EXTERNAL_V4} peer ${INTERNAL_V4} dev ${VETH_PREFIX}_ext scope link
ip address change local ${EXTERNAL_V6} peer ${INTERNAL_V6} dev ${VETH_PREFIX}_ext scope link

ip -n $NETNS_NAME address add local ${INTERNAL_V4} peer ${EXTERNAL_V4} dev ${VETH_PREFIX}_int scope link
ip -n $NETNS_NAME address change local ${INTERNAL_V6} peer ${EXTERNAL_V6} dev ${VETH_PREFIX}_int scope link
ip -n $NETNS_NAME route add default via ${EXTERNAL_V4} dev ${VETH_PREFIX}_int
ip -n $NETNS_NAME route add default via ${EXTERNAL_V6} dev ${VETH_PREFIX}_int

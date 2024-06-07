#!/bin/bash
VETH_PREFIX=cap
NETNS_NAME=capture_ns

ip link delete ${VETH_PREFIX}_ext
ip -n $NETNS_NAME link delete ${VETH_PREFIX}_int
ip netns delete $NETNS_NAME

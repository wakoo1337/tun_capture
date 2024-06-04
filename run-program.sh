#!/bin/sh
TUN_NAME=capture_tun
NETNS_NAME=capture_ns

ip netns add $NETNS_NAME
ip link set dev $TUN_NAME netns $NETNS_NAME
ip netns exec $NETNS_NAME "$@"

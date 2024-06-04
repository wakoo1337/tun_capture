#!/bin/sh
TUN_NAME=capture_tun
NETNS_NAME=capture_ns

ip netns delete $NETNS_NAME
ip tuntap del mode tun name $TUN_NAME

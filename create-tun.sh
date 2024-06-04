#!/bin/sh
TUN_NAME=capture_tun
USER_OWNER=wakoo
GROUP_OWNER=wakoo

ip tuntap add mode tun user $USER_OWNER group $GROUP_OWNER name $TUN_NAME
ip address add 192.168.100.10/24 dev $TUN_NAME
ip address add fe80::2222/64 dev $TUN_NAME

#!/bin/bash
NETNS_NAME=capture_ns

ip netns exec $NETNS_NAME "$@"

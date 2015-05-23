#!/bin/bash
#TODO: replace this with Makefile
set -x

#change this to your repo location
DOGE_DIR=/home/bunney/Documents/repos/DOGE

cd $DOGE_DIR/nodeCtrl/core/protocol
gcc -c -I. -I.. -fpic protocol.c
gcc -c -I. -I.. -fpic packet.c
cd ../memory_map
gcc -c -fpic memory_map.c
cd -
gcc -shared -o libprotocol.so protocol.o ../memory_map/memory_map.o packet.o
cd $DOGE_DIR/linux/doge/core
ln -s $DOGE_DIR/nodeCtrl/core/protocol/libprotocol.so libprotocol.so

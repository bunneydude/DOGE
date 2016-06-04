#!/bin/bash
# Radio Proxy Edison Arduino setup script. 
# This script symlinks required DOGE nodeCtrl source and header files into
# SKETCH libraries directory.

#SKETCH libraries directory

SKETCH_LIB_DIR=/home/Mario/Documents/Arduino/libraries

RUNENV=$(uname -o)

if [[ "$SKETCH_LIB_DIR" == "" ]]
  then
    echo "ERROR: SKETCH_LIB_DIR is undefined"
    exit 1
fi

if [[ ! -e "$SKETCH_LIB_DIR" ]]
  then
    echo "ERROR: SKETCH_LIB_DIR does not exist"
    exit 1
fi

if [[ ! -e "$PWD/linux/libs/IPCBuffer/sketch/IPCBuffer.cpp" ]]
  then
	  echo "ERROR: IPCBuffer library does not exist. Please run 'git submodule init' and 'git submodule update linux/libs/IPCBuffer'"
    exit 1
fi

if [[ $RUNENV == "Cygwin" ]]
  then
    echo "Please make sure this cygwin shell is run with administrator privileges."
    export CYGWIN=winsymlinks:native
fi

if [[ "$1" == "clean" ]]
  then
    rm -rf $SKETCH_LIB_DIR/*
    rm -rf $SKETCH_LIB_DIR/../radio_proxy
    exit 0
fi

mkdir -p $SKETCH_LIB_DIR/../radio_proxy
mkdir -p $SKETCH_LIB_DIR/doge_timers
mkdir -p $SKETCH_LIB_DIR/protocol
mkdir -p $SKETCH_LIB_DIR/IPCBuffer

#Use hard links for ino project files and symbolic links for library files
ln $PWD/nodes/edisonArduino/radio_proxy.ino                    $SKETCH_LIB_DIR/../radio_proxy/radio_proxy.ino
ln $PWD/nodes/edisonArduino/radio_proxy.h                      $SKETCH_LIB_DIR/../radio_proxy/radio_proxy.h
ln -s $PWD/nodeCtrl/core/platform/doge_timers.h                $SKETCH_LIB_DIR/doge_timers/doge_timers.h
ln -s $PWD/nodeCtrl/core/platform/doge_timers.c                $SKETCH_LIB_DIR/doge_timers/doge_timers.c
ln -s $PWD/nodeCtrl/core/protocol/type.h                       $SKETCH_LIB_DIR/protocol/type.h
ln -s $PWD/linux/libs/IPCBuffer/sketch/IPCBuffer.h             $SKETCH_LIB_DIR/IPCBuffer/IPCBuffer.h
ln -s $PWD/linux/libs/IPCBuffer/sketch/IPCBuffer.cpp           $SKETCH_LIB_DIR/IPCBuffer/IPCBuffer.cpp

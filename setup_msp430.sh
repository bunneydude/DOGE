#!/bin/bash
# nodeCtrl Energia setup script. 
# This script symlinks required DOGE nodeCtrl source and header files into
# Energia libraries directory.

#Energia libraries directory
ENERGIA_LIB_DIR=/home/bunney/Documents/energia_sketchbook/libraries/
#/c/Users/Mario/Documents/Energia/libraries

RUNENV=$(uname -o)

if [[ $RUNENV == "Cygwin" ]] 
  then
    echo "Please make sure this cygwin shell is run with administrator privileges."
    export CYGWIN=winsymlinks:native
fi

if [[ "$1" == "clean" ]]
  then
    rm -rf $ENERGIA_LIB_DIR/*
    rm -rf $ENERGIA_LIB_DIR/../nodeCtrl
    NODE_CTRL_PATH="$PWD/nodes/msp430g2553/nodeCtrl_v1"
    GIT_PATH="$NODE_CTRL_PATH/nodeCtrl.ino"
    MOVED_DIR="$NODE_CTRL_PATH/nodeCtrl"
    MOVED_PATH="$MOVED_DIR/nodeCtrl.ino"
    #Delete any empty nodeCtrl directories
    find "$NODE_CTRL_PATH" -depth -type d -empty -delete
    #Check to see if Energia moved nodeCtrl.ino
    if [[ -e $MOVED_PATH ]] && [[ ! -e $GIT_PATH ]]
      then
        mv "$MOVED_PATH" "$GIT_PATH"
        rm -rf "$MOVED_DIR"
    elif [[ -e "$MOVED_PATH" ]] && [[ -e "$GIT_PATH" ]]
      then
        NODE_CTRL_DIFF=$(diff "$GIT_PATH" "$MOVED_PATH")
        if [[ "$NODE_CTRL_DIFF" != "" ]]
          then
            echo "Could not erase $MOVED_PATH"
            echo "$NODE_CTRL_DIFF"
        else
            rm -rf "$MOVED_DIR"
        fi
    elif [[ ! -e "$GIT_PATH" ]]
      then
        git checkout "$GIT_PATH"
    fi
    exit 0
fi

mkdir -p $ENERGIA_LIB_DIR/../nodeCtrl
mkdir -p $ENERGIA_LIB_DIR/nrf24
mkdir -p $ENERGIA_LIB_DIR/adc
mkdir -p $ENERGIA_LIB_DIR/neighbor
mkdir -p $ENERGIA_LIB_DIR/protocol
mkdir -p $ENERGIA_LIB_DIR/gpio
mkdir -p $ENERGIA_LIB_DIR/dsp
mkdir -p $ENERGIA_LIB_DIR/memory_map
mkdir -p $ENERGIA_LIB_DIR/routing
mkdir -p $ENERGIA_LIB_DIR/task
mkdir -p $ENERGIA_LIB_DIR/network
mkdir -p $ENERGIA_LIB_DIR/platform
mkdir -p $ENERGIA_LIB_DIR/cobs

ln -s $PWD/nodes/msp430g2553/nodeCtrl_v1/nodeCtrl.ino        $ENERGIA_LIB_DIR/../nodeCtrl/nodeCtrl.ino
ln -s $PWD/nodeCtrl/core/adc/adc.h                           $ENERGIA_LIB_DIR/adc/adc.h
ln -s $PWD/nodeCtrl/core/adc/adc.c                           $ENERGIA_LIB_DIR/adc/adc.c
ln -s $PWD/nodeCtrl/core/neighbor/neighbor-config.h          $ENERGIA_LIB_DIR/neighbor/neighbor-config.h
ln -s $PWD/nodeCtrl/core/neighbor/neighbor.h                 $ENERGIA_LIB_DIR/neighbor/neighbor.h
ln -s $PWD/nodeCtrl/core/nodeCtrl_errno.h                    $ENERGIA_LIB_DIR/nodeCtrl_errno.h
ln -s $PWD/nodeCtrl/core/protocol/protocol.h                 $ENERGIA_LIB_DIR/protocol/protocol.h
ln -s $PWD/nodeCtrl/core/protocol/protocol.c                 $ENERGIA_LIB_DIR/protocol/protocol.c
ln -s $PWD/nodeCtrl/core/protocol/packet.c                   $ENERGIA_LIB_DIR/protocol/packet.c
ln -s $PWD/nodeCtrl/core/protocol/type.h                     $ENERGIA_LIB_DIR/protocol/type.h
ln -s $PWD/nodeCtrl/core/radios/nrf24l01p/nrf24l01p-const.h  $ENERGIA_LIB_DIR/nrf24/nrf24l01p-const.h
ln -s $PWD/nodeCtrl/core/radios/nrf24l01p/nrf24l01p.h        $ENERGIA_LIB_DIR/nrf24/nrf24l01p.h
ln -s $PWD/nodeCtrl/core/radios/nrf24l01p/nrf24l01p.c        $ENERGIA_LIB_DIR/nrf24/nrf24l01p.c
ln -s $PWD/nodeCtrl/core/gpio/msp430_gpio.h                  $ENERGIA_LIB_DIR/gpio/msp430_gpio.h
ln -s $PWD/nodeCtrl/core/gpio/gpio.c                         $ENERGIA_LIB_DIR/gpio/gpio.c
ln -s $PWD/nodeCtrl/core/gpio/gpio.h                         $ENERGIA_LIB_DIR/gpio/gpio.h
ln -s $PWD/nodeCtrl/core/dsp/dsp.h                           $ENERGIA_LIB_DIR/dsp/dsp.h
ln -s $PWD/nodeCtrl/core/dsp/dsp.c                           $ENERGIA_LIB_DIR/dsp/dsp.c
ln -s $PWD/nodeCtrl/core/memory_map/memory_map.h             $ENERGIA_LIB_DIR/memory_map/memory_map.h
ln -s $PWD/nodeCtrl/core/memory_map/memory_map.c             $ENERGIA_LIB_DIR/memory_map/memory_map.c
ln -s $PWD/nodeCtrl/core/routing/routing.h                   $ENERGIA_LIB_DIR/routing/routing.h
ln -s $PWD/nodeCtrl/core/packet.h                            $ENERGIA_LIB_DIR/packet.h
ln -s $PWD/nodeCtrl/core/nodeCtrl.h                          $ENERGIA_LIB_DIR/nodeCtrl.h
ln -s $PWD/nodeCtrl/core/task/task_list.h                    $ENERGIA_LIB_DIR/task/task_list.h
ln -s $PWD/nodeCtrl/core/task/task_list.c                    $ENERGIA_LIB_DIR/task/task_list.c
ln -s $PWD/nodeCtrl/core/network/network.h                   $ENERGIA_LIB_DIR/network/network.h
ln -s $PWD/nodeCtrl/core/network/network.c                   $ENERGIA_LIB_DIR/network/network.c
ln -s $PWD/nodeCtrl/core/platform/serial_c.h                 $ENERGIA_LIB_DIR/platform/serial_c.h
ln -s $PWD/nodeCtrl/core/platform/serial_c.cpp               $ENERGIA_LIB_DIR/platform/serial_c.cpp
ln -s $PWD/nodeCtrl/core/platform/reliable_channel.h         $ENERGIA_LIB_DIR/platform/reliable_channel.h
ln -s $PWD/nodeCtrl/core/platform/reliable_channel.cpp       $ENERGIA_LIB_DIR/platform/reliable_channel.cpp
ln -s $PWD/nodeCtrl/core/platform/doge_timers.h              $ENERGIA_LIB_DIR/platform/doge_timers.h
ln -s $PWD/nodeCtrl/core/platform/doge_timers.c              $ENERGIA_LIB_DIR/platform/doge_timers.c
ln -s $PWD/nodeCtrl/core/platform/platform.h                 $ENERGIA_LIB_DIR/platform/platform.h
ln -s $PWD/nodeCtrl/core/cobs/cobs.c                         $ENERGIA_LIB_DIR/cobs/cobs.cpp
ln -s $PWD/nodeCtrl/core/cobs/cobs.h                         $ENERGIA_LIB_DIR/cobs/cobs.h

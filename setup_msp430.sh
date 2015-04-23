#!/bin/bash
# nodeCtrl Energia setup script. 
# This script symlinks required DOGE nodeCtrl source and header files into
# Energia libraries directory.

#Energia libraries directory
ENERGIA_LIB_DIR=/c/Users/Mario/Documents/Energia/libraries

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
		git checkout $PWD/nodes/msp430g2553/nodeCtrl_v1/nodeCtrl.ino
		if [[ -e $PWD/nodes/msp430g2553/nodeCtrl_v1/nodeCtrl/nodeCtrl.ino ]]
			then
				NODE_CTRL_DIFF=$(diff $PWD/nodes/msp430g2553/nodeCtrl_v1/nodeCtrl.ino $PWD/nodes/msp430g2553/nodeCtrl_v1/nodeCtrl/nodeCtrl.ino)
				if [[ "$NODE_CTRL_DIFF" != "" ]]
					then
						echo "Could not erase $PWD/nodes/msp430g2553/nodeCtrl_v1/nodeCtrl/nodeCtrl.ino"
						echo "$NODE_CTRL_DIFF"
				else
						rm -rf $PWD/nodes/msp430g2553/nodeCtrl_v1/nodeCtrl
				fi
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

ln -s $PWD/nodes/msp430g2553/nodeCtrl_v1/nodeCtrl.ino        $ENERGIA_LIB_DIR/../nodeCtrl/nodeCtrl.ino
ln -s $PWD/nodeCtrl/core/adc/adc.h                           $ENERGIA_LIB_DIR/adc/adc.h
ln -s $PWD/nodeCtrl/core/adc/adc.c                           $ENERGIA_LIB_DIR/adc/adc.c
ln -s $PWD/nodeCtrl/core/neighbor/neighbor-config.h          $ENERGIA_LIB_DIR/neighbor/neighbor-config.h
ln -s $PWD/nodeCtrl/core/neighbor/neighbor.h                 $ENERGIA_LIB_DIR/neighbor/neighbor.h
ln -s $PWD/nodeCtrl/core/neighbor/neighbor.c                 $ENERGIA_LIB_DIR/neighbor/neighbor.c
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

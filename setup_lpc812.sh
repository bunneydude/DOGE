#!/bin/bash
# nodeCtrl XPRESSO setup script. 
# This script symlinks required DOGE nodeCtrl source and header files into
# XPRESSO WORKSPACE directory.

#Xpresso workspace directory

XPRESSO_WORKSPACE_DIR=/home/Mario/Documents/LPCXpresso_7.7.2_379/workspace/lpc812_nodeCtrl

RUNENV=$(uname -o)

if [[ "$XPRESSO_WORKSPACE_DIR" == "" ]]
  then
	  echo "ERROR: XPRESSO_WORKSPACE_DIR is undefined"
    exit 1
fi

if [[ $RUNENV == "Cygwin" ]] 
  then
    echo "Please make sure this cygwin shell is run with administrator privileges."
    export CYGWIN=winsymlinks:native
fi

if [[ ! -e "$XPRESSO_WORKSPACE_DIR" ]]
  then
    echo "ERROR: XPRESSO_WORKSPACE_DIR does not exist"
    exit 1
fi

if [[ "$1" == "clean" ]]
  then
		echo "clean command is not supported. Please delete Xpresso project."
    exit 0
fi

mkdir -p $XPRESSO_WORKSPACE_DIR/src/nrfLegacy
mkdir -p $XPRESSO_WORKSPACE_DIR/src/adc
mkdir -p $XPRESSO_WORKSPACE_DIR/src/neighbor
mkdir -p $XPRESSO_WORKSPACE_DIR/src/protocol
mkdir -p $XPRESSO_WORKSPACE_DIR/src/gpio
mkdir -p $XPRESSO_WORKSPACE_DIR/src/dsp
mkdir -p $XPRESSO_WORKSPACE_DIR/src/memory_map
mkdir -p $XPRESSO_WORKSPACE_DIR/src/routing
mkdir -p $XPRESSO_WORKSPACE_DIR/src/task
mkdir -p $XPRESSO_WORKSPACE_DIR/src/network
mkdir -p $XPRESSO_WORKSPACE_DIR/src/platform
mkdir -p $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/gpio
mkdir -p $XPRESSO_WORKSPACE_DIR/src/radios

ln -s $PWD/nodes/lpc812/lpc812_nodeCtrl/src/main.c            $XPRESSO_WORKSPACE_DIR/src/main.c
ln -s $PWD/nodes/lpc812/lpc812_nodeCtrl/src/main.h            $XPRESSO_WORKSPACE_DIR/src/main.h
ln -s $PWD/nodes/lpc812/lpc812_nodeCtrl/src/lpc_type.h        $XPRESSO_WORKSPACE_DIR/src/lpc_type.h
ln -s $PWD/nodeCtrl/core/adc/adc.h                            $XPRESSO_WORKSPACE_DIR/src/adc/adc.h
ln -s $PWD/nodeCtrl/core/adc/adc.c                            $XPRESSO_WORKSPACE_DIR/src/adc/adc.c
ln -s $PWD/nodeCtrl/core/neighbor/neighbor-config.h           $XPRESSO_WORKSPACE_DIR/src/neighbor/neighbor-config.h
ln -s $PWD/nodeCtrl/core/neighbor/neighbor.h                  $XPRESSO_WORKSPACE_DIR/src/neighbor/neighbor.h
ln -s $PWD/nodeCtrl/core/nodeCtrl_errno.h                     $XPRESSO_WORKSPACE_DIR/src/nodeCtrl_errno.h
ln -s $PWD/nodeCtrl/core/protocol/protocol.h                  $XPRESSO_WORKSPACE_DIR/src/protocol/protocol.h
ln -s $PWD/nodeCtrl/core/protocol/protocol.c                  $XPRESSO_WORKSPACE_DIR/src/protocol/protocol.c
ln -s $PWD/nodeCtrl/core/protocol/packet.c                    $XPRESSO_WORKSPACE_DIR/src/protocol/packet.c
ln -s $PWD/nodeCtrl/core/protocol/type.h                      $XPRESSO_WORKSPACE_DIR/src/protocol/type.h
ln -s $PWD/nodeCtrl/core/radios/nrfLegacy/nRF24L01.h          $XPRESSO_WORKSPACE_DIR/src/nrfLegacy/nRF24L01.h
ln -s $PWD/nodeCtrl/core/radios/nrfLegacy/nrf24.c             $XPRESSO_WORKSPACE_DIR/src/nrfLegacy/nrf24.c
ln -s $PWD/nodeCtrl/core/radios/nrfLegacy/nrf24.h             $XPRESSO_WORKSPACE_DIR/src/nrfLegacy/nrf24.h
ln -s $PWD/nodeCtrl/core/radios/nrfLegacy/nrfLegacy.h         $XPRESSO_WORKSPACE_DIR/src/nrfLegacy/nrfLegacy.h
ln -s $PWD/nodeCtrl/core/radios/radios.h                      $XPRESSO_WORKSPACE_DIR/src/radios/radios.h
ln -s $PWD/nodeCtrl/core/gpio/gpio.c                          $XPRESSO_WORKSPACE_DIR/src/gpio/gpio.c
ln -s $PWD/nodeCtrl/core/gpio/gpio.h                          $XPRESSO_WORKSPACE_DIR/src/gpio/gpio.h
ln -s $PWD/nodeCtrl/core/dsp/dsp.h                            $XPRESSO_WORKSPACE_DIR/src/dsp/dsp.h
ln -s $PWD/nodeCtrl/core/dsp/dsp.c                            $XPRESSO_WORKSPACE_DIR/src/dsp/dsp.c
ln -s $PWD/nodeCtrl/core/memory_map/memory_map.h              $XPRESSO_WORKSPACE_DIR/src/memory_map/memory_map.h
ln -s $PWD/nodeCtrl/core/memory_map/memory_map.c              $XPRESSO_WORKSPACE_DIR/src/memory_map/memory_map.c
ln -s $PWD/nodeCtrl/core/routing/routing.h                    $XPRESSO_WORKSPACE_DIR/src/routing/routing.h
ln -s $PWD/nodeCtrl/core/packet.h                             $XPRESSO_WORKSPACE_DIR/src/packet.h
ln -s $PWD/nodeCtrl/core/nodeCtrl.h                           $XPRESSO_WORKSPACE_DIR/src/nodeCtrl.h
ln -s $PWD/nodeCtrl/core/nodeCtrl.c                           $XPRESSO_WORKSPACE_DIR/src/nodeCtrl.c
ln -s $PWD/nodeCtrl/core/task/task_list.h                     $XPRESSO_WORKSPACE_DIR/src/task/task_list.h
ln -s $PWD/nodeCtrl/core/task/task_list.c                     $XPRESSO_WORKSPACE_DIR/src/task/task_list.c
ln -s $PWD/nodeCtrl/core/network/network.h                    $XPRESSO_WORKSPACE_DIR/src/network/network.h
ln -s $PWD/nodeCtrl/core/network/network.c                    $XPRESSO_WORKSPACE_DIR/src/network/network.c
ln -s $PWD/nodeCtrl/core/platform/serial_c.h                  $XPRESSO_WORKSPACE_DIR/src/platform/serial_c.h
ln -s $PWD/nodeCtrl/core/platform/serial_c.cpp                $XPRESSO_WORKSPACE_DIR/src/platform/serial_c.c
ln -s $PWD/nodeCtrl/core/platform/reliable_channel.h          $XPRESSO_WORKSPACE_DIR/src/platform/reliable_channel.h
ln -s $PWD/nodeCtrl/core/platform/reliable_channel.cpp        $XPRESSO_WORKSPACE_DIR/src/platform/reliable_channel.c
ln -s $PWD/nodeCtrl/core/platform/doge_gpio.h                 $XPRESSO_WORKSPACE_DIR/src/platform/doge_gpio.h
ln -s $PWD/nodeCtrl/core/platform/doge_gpio.c                 $XPRESSO_WORKSPACE_DIR/src/platform/doge_gpio.c
ln -s $PWD/nodeCtrl/core/platform/doge_timers.h               $XPRESSO_WORKSPACE_DIR/src/platform/doge_timers.h
ln -s $PWD/nodeCtrl/core/platform/doge_timers.c               $XPRESSO_WORKSPACE_DIR/src/platform/doge_timers.c
ln -s $PWD/nodeCtrl/core/platform/static_routes.cpp           $XPRESSO_WORKSPACE_DIR/src/platform/static_routes.c
ln -s $PWD/nodeCtrl/core/platform/static_routes.h             $XPRESSO_WORKSPACE_DIR/src/platform/static_routes.h
ln -s $PWD/nodeCtrl/core/platform/nrf24_wrappers.cpp          $XPRESSO_WORKSPACE_DIR/src/platform/nrf24_wrappers.c
ln -s $PWD/nodeCtrl/core/platform/platform.h                  $XPRESSO_WORKSPACE_DIR/src/platform/platform.h
ln -s $PWD/nodeCtrl/core/platform/doge_radio.h                $XPRESSO_WORKSPACE_DIR/src/platform/doge_radio.h
ln -s $PWD/nodeCtrl/core/platform/doge_radio.cpp              $XPRESSO_WORKSPACE_DIR/src/platform/doge_radio.c
ln -s $PWD/nodeCtrl/core/platform/doge_adc.h                  $XPRESSO_WORKSPACE_DIR/src/platform/doge_adc.h
ln -s $PWD/nodeCtrl/core/platform/doge_adc.c                  $XPRESSO_WORKSPACE_DIR/src/platform/doge_adc.c
ln -s $PWD/nodeCtrl/core/platform/lpc812/gpio/gpio.h          $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/gpio/gpio.h
ln -s $PWD/nodeCtrl/core/platform/lpc812/gpio/gpio.c          $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/gpio/gpio.c
ln -s $PWD/nodeCtrl/core/platform/lpc812/uart.c               $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/uart.c
ln -s $PWD/nodeCtrl/core/platform/lpc812/uart.h               $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/uart.h
ln -s $PWD/nodeCtrl/core/platform/lpc812/spi.c                $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/spi.c
ln -s $PWD/nodeCtrl/core/platform/lpc812/spi.h                $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/spi.h
ln -s $PWD/nodeCtrl/core/platform/lpc812/pwm.c                $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/pwm.c
ln -s $PWD/nodeCtrl/core/platform/lpc812/pwm.h                $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/pwm.h
ln -s $PWD/nodeCtrl/core/platform/lpc812/printf.c             $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/printf.c
ln -s $PWD/nodeCtrl/core/platform/lpc812/printf.h             $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/printf.h
ln -s $PWD/nodeCtrl/core/platform/lpc812/printf-retarget.c    $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/printf-retarget.c
ln -s $PWD/nodeCtrl/core/platform/lpc812/swm.c                $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/swm.c
ln -s $PWD/nodeCtrl/core/platform/lpc812/initializer.h        $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/initializer.h
ln -s $PWD/nodeCtrl/core/platform/lpc812/mrt.c                $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/mrt.c
ln -s $PWD/nodeCtrl/core/platform/lpc812/mrt.h                $XPRESSO_WORKSPACE_DIR/src/platform/lpc812/mrt.h
ln -s $PWD/nodeCtrl/core/platform/hw_init.h                   $XPRESSO_WORKSPACE_DIR/src/platform/hw_init.h
ln -s $PWD/nodeCtrl/core/platform/doge_app.c                  $XPRESSO_WORKSPACE_DIR/src/platform/doge_app.c
ln -s $PWD/nodeCtrl/core/platform/doge_app.h                  $XPRESSO_WORKSPACE_DIR/src/platform/doge_app.h

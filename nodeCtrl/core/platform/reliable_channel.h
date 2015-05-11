#ifndef RELIABLE_CHANNEL_H
#define RELIABLE_CHANNEL_H
#include "platform.h"
#include "../protocol/type.h"
#include "../protocol/protocol.h"
#include "../packet.h"
#ifdef MSP430
#include <SPI.h>
#include <AIR430BoostFCC.h>
#endif

#define TIMEOUT_500_MS (12000/2)
#define TIMEOUT_1000_MS (12000)

#if 0 //DEBUG SETTINGS
#define RETRY_COUNT (2)
#define RETRY_TIMER_INTERVAL (12000/5) //200ms
#define RX_TIMER_TIMEOUT (RETRY_TIMER_INTERVAL * (1 + RETRY_COUNT)) //600ms
#define RADIO_RX_TIMEOUT (200) //radio.ReceiverOn will actually timeout 200-300ms
#else //RELEASE SETTINGS 
//DO NOT PRINT ANYTHING IN RELIABLE RECEIVE/TRANSMIT TO USE THESE TIMINGS
#define RETRY_COUNT (2)
#define RETRY_TIMER_INTERVAL (12000/50) //20ms
#define RX_TIMER_TIMEOUT (RETRY_TIMER_INTERVAL * (1 + RETRY_COUNT)) //60ms
#define RADIO_RX_TIMEOUT (20) //20ms
#endif

dogeBool reliable_transmit();
dogeBool reliable_receive(uint16_t timeout);

#endif

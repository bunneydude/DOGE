#ifndef RADIO_PROXY_H
#define RADIO_PROXY_H

#include <stdint.h>
#include <ringBuffer.h>

#define HBT_LED 13

#define TASK_HBT_PERIOD 13
#define COUNTER_RATE 40 //ms
#define SERIAL_BAUD 115200
//counters
volatile uint16_t task_hbt = TASK_HBT_PERIOD;

#define RX_BUFFER_LENGTH 8
#define RX_BUFFER_WIDTH 1

#define CMD_BUFFER_LENGTH 8
#define CMD_BUFFER_WIDTH 1

#define PRODUCERS 1
#define CONSUMERS 1

//a union for accessing an address as two bytes or one two byte int
typedef union Addressu
{
  uint16_t u1;
  uint8_t u2[2];
}Address;

#endif




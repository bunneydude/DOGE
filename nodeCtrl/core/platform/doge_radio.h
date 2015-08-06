#ifndef DOGE_RADIO_H
#define DOGE_RADIO_H

#include <stdint.h>

#ifdef MSP430
#include <Energia.h>
#endif

#ifdef __LPC8XX__
uint8_t sending();
uint8_t get_data(uint8_t* data, uint8_t length, uint16_t timeout);
void send_data(uint8_t address, uint8_t* data, uint8_t length);
int8_t get_rssi();
#elif defined(MSP430)
static uint8_t sending() __attribute__ ((weakref("_ZN14A110x2500Radio4busyEv")));
static uint8_t get_data(uint8_t* data, uint8_t length, uint16_t timeout) __attribute__ ((weakref("_ZN14A110x2500Radio10receiverOnEPhhj")));
static void send_data(uint8_t address, uint8_t* data, uint8_t length) __attribute__ ((weakref("_ZN14A110x2500Radio8transmitEhPhh")));
static int8_t get_rssi() __attribute__ ((weakref("_ZN14A110x2500Radio7getRssiEv")));
#endif
#endif

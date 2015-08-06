#ifndef DOGE_ADC_H
#define DOGE_ADC_H
#include <stdint.h>
#ifdef __LPC8XX__
uint16_t analog_read(uint8_t pin);
#elif defined(MSP430)
static uint16_t analog_read(uint8_t pin) __attribute__ ((weakref("analogRead")));
#endif
#endif

#ifndef DOGE_RADIO_H
#define DOGE_RADIO_H

#include <stdint.h>
#include "../nrfLegacy/nrfLegacy.h"
#ifdef __LPC8XX__
#undef CONFIG
#endif
#include <platform.h>

#ifdef MSP430
#include <Energia.h>
#endif

#if !RADIO_2400_EN && !RADIO_915_EN && !RADIO_433_EN
#error "No radios are enabled"
#elif !RADIO_2400_EN && !RADIO_915_EN && RADIO_433_EN
#define NUM_RADIOS 1
#define RADIO_ID_433 0
#define RADIO_ID_ALL 0
#elif !RADIO_2400_EN && RADIO_915_EN && !RADIO_433_EN
#define NUM_RADIOS 1
#define RADIO_ID_915 0
#define RADIO_ID_ALL 0
#elif !RADIO_2400_EN && RADIO_915_EN && RADIO_433_EN
#define NUM_RADIOS 3
#define RADIO_ID_433 0
#define RADIO_ID_915 1
#define RADIO_ID_ALL 2
#elif RADIO_2400_EN && !RADIO_915_EN && !RADIO_433_EN
#define NUM_RADIOS 1
#define RADIO_ID_2400 0
#define RADIO_ID_ALL 0
#elif RADIO_2400_EN && !RADIO_915_EN && RADIO_433_EN
#define NUM_RADIOS 3
#define RADIO_ID_433 0
#define RADIO_ID_2400 1
#define RADIO_ID_ALL 2
#elif RADIO_2400_EN && RADIO_915_EN && !RADIO_433_EN
#define NUM_RADIOS 3
#define RADIO_ID_915 0
#define RADIO_ID_2400 1
#define RADIO_ID_ALL 2
#elif RADIO_2400_EN && RADIO_915_EN && RADIO_433_EN
#define NUM_RADIOS 4
#define RADIO_ID_433 0
#define RADIO_ID_915 1
#define RADIO_ID_2400 2
#define RADIO_ID_ALL 3
#endif

#ifdef __LPC8XX__
int8_t empty_get_rssi();
#endif
#endif

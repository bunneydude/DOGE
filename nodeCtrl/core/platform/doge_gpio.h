#ifndef DOGE_GPIO_H
#define DOGE_GPIO_H
#include "doge_timers.h"

#ifdef MSP430
#include <Energia.h>
#endif

#define LED_OFF_VALUE (0)
#define LED_ON_VALUE  (1)
#define NUM_BLINKS (1)

typedef enum {
   LED_RESET,
   LED_START_TOGGLE,
   LED_ON,
   LED_OFF,
   LED_INC_COUNTER
}dogeLEDState;

void toggle_led(dogeBool init);

#endif

#ifndef DOGE_GPIO_H
#define DOGE_GPIO_H
#include "doge_timers.h"

#ifdef __LPC8XX__
#include "lpc812/gpio/gpio.h"
#elif defined(MSP430)
#include <Energia.h>
#endif

#define LED_OFF_VALUE (0)
#define LED_ON_VALUE  (1)
#define NUM_BLINKS (2)

typedef enum {
   LED_RESET,
   LED_START_TOGGLE,
   LED_ON,
   LED_OFF,
   LED_INC_COUNTER
}dogeLEDState;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef __LPC8XX__
void digital_write(uint8_t pin, uint8_t value);
int digital_read(uint8_t pin);
#elif defined(MSP430)
static void digital_write(uint8_t pin, uint8_t value) __attribute__ ((weakref("digitalWrite")));
static int digital_read(uint8_t pin) __attribute__ ((weakref("digitalRead")));
#endif
void toggle_led(dogeBool init);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

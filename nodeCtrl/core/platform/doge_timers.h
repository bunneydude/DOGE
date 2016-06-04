#ifndef DOGE_TIMERS_H
#define DOGE_TIMERS_H
#include <stdint.h>
#include "../protocol/type.h"
#include "serial_c.h"

#if defined(LINUX) || defined(__ARDUINO_X86__)
#include <time.h>
#endif
#ifdef MSP430
#include <msp430g2553.h>
#include <doge_gpio.h>
#endif
#ifdef __LPC8XX__
#include <lpc812/mrt.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(__LPC8XX__) || defined(LINUX) || defined(__ARDUINO_X86__)
typedef uint32_t timerType;
#define MAX_TIMER_VAL       (UINT32_MAX)
#elif defined(MSP430)
typedef uint16_t timerType;
#define MAX_TIMER_VAL       (UINT16_MAX)
#endif

#define TIMER_OVERFLOW(a,b) ((MAX_TIMER_VAL - (a)) < (b))
#define TIMER_BEGIN(timer)  (((timer)->end)-((timer)->duration))
#define TIMER_END(timer)    ((timer)->end)

typedef struct
{
   timerType duration;
   timerType end;
}dogeTimer;

#define THRESHOLD 50
/* Energia wiring_analog.c settings */
/*uint16_t analog_reference = DEFAULT, analog_period = F_CPU/490, analog_div = ID_0, analog_res=255; // devide clock with 0, 2, 4, 8*/
#define ANALOG_DIV (ID_0)
#define ANALOG_RES (255)
#define PWM_PERIOD (F_CPU)/490 // F_CPU/490
#define PWM_DUTY(x) ( (unsigned long)x*PWM_PERIOD / (unsigned long)ANALOG_RES )

#ifdef __LPC8XX__
#define TIMEOUT_500_MS (500)
#define TIMEOUT_1000_MS (1000)
#elif defined(MSP430)
#define TIMEOUT_500_MS (12000/2)
#define TIMEOUT_250_MS (12000/4)
#define TIMEOUT_100_MS (12000/10)
#define TIMEOUT_1000_MS (12000)
#endif

timerType current_time();
void timer_init(dogeTimer* timer, timerType duration);
dogeBool timer_expired(dogeTimer* timer);
void timer_reset(dogeTimer* timer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

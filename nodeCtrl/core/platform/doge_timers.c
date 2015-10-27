#include "doge_timers.h"
  
#ifdef MSP430
void setup_timer_hw()
{
   /*
   * This setup code is similar to wiring_analog.c analogWrite
   * Differences:
   * 1. Changed TACCR0 to count to MAX TIMER VAL
   * 2. Changed clock source to ACLK instead of SMCLK
   */
   TA0CCR0 = MAX_TIMER_VAL;       // PWM Period
   TA0CCTL1 = OUTMOD_7;           // reset/set
   TA0CCR1 = PWM_DUTY(100);       // PWM duty cycle
   TA0CTL = TASSEL_1 + MC_1 + ANALOG_DIV;       // ACLK, up mode
}
#endif
#if defined(LINUX) || defined(__ARDUINO_X86__)
void setup_timer_hw(){}
#endif

dogeBool timer_expired(dogeTimer* timer)
{
   timerType currTime = current_time();
   dogeBool expired = TRUE;
   if ((currTime < TIMER_END(timer)) && (currTime >= TIMER_BEGIN(timer))) {
      expired = FALSE;
   }
   // Overflow
   else if (TIMER_OVERFLOW(TIMER_BEGIN(timer), timer->duration)) {
      if ((currTime >= TIMER_BEGIN(timer) && currTime > TIMER_END(timer)) ||
          (currTime < TIMER_BEGIN(timer)  && currTime < TIMER_END(timer))) {
         expired = FALSE;
      }
   }
   /*
    *print_string("Timer Current: ");
    *print_bytes((uint8_t*)&currTime);
    *print_string("Timer End: ");
    *print_bytes((uint8_t*)&timer->end);
    */

   //printf("Timer Current: %d, Timer end: %d Timer->end - Timer->duration %x, Expired: %d \n", currTime, timer->end, (timer->end - timer->duration), expired);
   return expired;
}

timerType current_time()
{
#if defined(LINUX) || defined(__ARDUINO_X86__)
   struct timespec ts;
   clock_gettime(CLOCK_REALTIME, &ts);
   double ms = (ts.tv_sec * 1e3) + (ts.tv_nsec * 1e-6);
   timerType value = (timerType)(((long long int)ms) % MAX_TIMER_VAL);
   return value;
#endif
#ifdef MSP430
   // based on timer_a.c TIMER_A_getCounterValue function 
   timerType sample1 = 0;
   timerType sample2 = 0;
   while(1)
   {
      sample2 = sample1;
      sample1 = TA0R;
      if ((sample1 > sample2) && (sample1 - sample2 < THRESHOLD))
      {
         break;
      }
      else if ((sample2 > sample1) && (sample2-sample1 < THRESHOLD))
      {
         break;
      }
   }
   return sample2;
#endif
#ifdef __LPC8XX__
   //test
   timerType sample1 = doge_counter;
   timerType sample2 = 0;
   while(1)
   {
      sample2 = sample1;
      sample1 = doge_counter;
      if ((sample1 > sample2) && (sample1 - sample2 < THRESHOLD))
      {
         break;
      }
      else if ((sample2 > sample1) && (sample2-sample1 < THRESHOLD))
      {
         break;
      }
   }
   return sample2;
#endif
}

void timer_init(dogeTimer* timer, timerType duration)
{
   timer->end = current_time() + duration;
   timer->duration = duration;
}

void timer_reset(dogeTimer* timer)
{
   timer->duration = 0;
   timer->end = 0;
}

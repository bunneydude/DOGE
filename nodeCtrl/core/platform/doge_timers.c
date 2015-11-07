#include "doge_timers.h"
  
#ifdef MSP430
void setup_timer_hw()
{
   TA0CCR0 = MAX_TIMER_VAL;
   TA0CCTL0 = OUTMOD_0; // Output only
   TA0CTL = TACLR + TASSEL_1 + MC_2 + ANALOG_DIV;       // ACLK, continuous mode
}

void restart_wdt()
{
   TA0CTL = TASSEL_1 + MC_2 + ANALOG_DIV + TAIE; // ACLK, continuous mode, Enable Timer A interrupts
   TA0R = 0;
}

/* DO NOT USE ENERGIA SERIAL PRINT LIBRARIES ANYWHERE WITHIN THIS ISR
 * (including toggle_led). THE LIBRARY WILL BE BLOCKED FROM USING INTERRUPTS
 * AND WILL HANG THE MSP. */
__attribute__((interrupt(TIMER0_A1_VECTOR)))
void timer_a0_overflow_isr (void)
{
    volatile uint16_t TAIVIFG; 
    TAIVIFG=TA0IV; (void)TAIVIFG; // just reading TAIV will reset the interrupt flag
    dogeTimer delay;
    timer_init(&delay, TIMEOUT_1000_MS);
    while(!timer_expired(&delay))
    {
      toggle_led(TRUE);
    }
    WDTCTL = 0xDEAD;
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

#include <stdint.h>
#include <stdio.h>
#include "neighbor.h"
#ifdef LINUX
#include <time.h>
#endif
#ifdef MSP430
#include <msp430g2553.h>
#endif

#define LENGTH 1

timerType current_time()
{
#ifdef LINUX
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
      if ((sample1 > sample2) && (sample1 - sample2 < 50))
      {
         break;
      }
      else if ((sample2 > sample1) && (sample2-sample1 < 50))
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

struct neighborEntry alpha[LENGTH];

#if DBG
void main()
{
   dogeTimer neighborDiscovery;
   timer_init(&neighborDiscovery, 3000);
   while(!timer_expired(&neighborDiscovery))
   {
      /*printf("waiting...");*/
   }

	uint8_t i = 0;
	for(i=0; i<LENGTH; i++){
		alpha[i].shNodeID = i;
		alpha[i].shLQE = i;
		alpha[i].radioID = i;
		alpha[i].networkID = i;
	}

	#ifdef LINUX
	printf("Alpha size = %d\n", sizeof(alpha));
	printf("Test size = %d\n", sizeof(i));
	#endif
}
#endif

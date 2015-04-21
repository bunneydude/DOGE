#include <stdint.h>
#include <stdio.h>
#include "neighbor.h"
#include "../protocol/type.h"
#ifndef MSP430
#define boolean int
#include <time.h>
#endif

#define LENGTH 1

typedef uint16_t timerType;
#define MAX_TIMER_VAL (UINT16_MAX) 

#define TIMER_OVERFLOW(a,b) ((MAX_TIMER_VAL - (a)) < (b))

typedef struct
{
   timerType duration;
   timerType end;
}dogeTimer;

timerType current_time()
{
#ifdef LINUX
   struct timespec ts;
   clock_gettime(CLOCK_REALTIME, &ts);
   double ms = (ts.tv_sec * 1e3) + (ts.tv_nsec * 1e-6);
   timerType value = (timerType)(((long long int)ms) % MAX_TIMER_VAL);
   return value;
#endif
}

void timer_init(dogeTimer* timer, timerType duration)
{
   timer->end = current_time() + duration;
   timer->duration = duration;
}

dogeBool timer_expired(dogeTimer* timer)
{
   timerType currTime = current_time();
   dogeBool expired = TRUE;
   if ((currTime < timer->end) && (currTime >= (timer->end - timer->duration)))
   {
      expired = FALSE;
   }
   // Overflow
   else if ((currTime > timer->end))
   {
      expired = FALSE;
   }
    printf("Timer Current: %d, Timer end: %d Timer->end - Timer->duration %x, Expired: %d \n", currTime, timer->end, (timer->end - timer->duration), expired);
   return expired;
}

void timer_reset(dogeTimer* timer)
{
   timer->duration = 0;
   timer->end = 0;
}

struct neighborEntry alpha[LENGTH];

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

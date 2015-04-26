#include <stdint.h>
#include <stdio.h>
#include "neighbor.h"
#include <platform.h>

#define LENGTH 1

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

#include <stdint.h>
#include <stdio.h>
#include "neighbor.h"

#define LENGTH 1

struct neighborEntry alpha[LENGTH];

void main()
{

	uint8_t i = 0;
	for(i=0; i<LENGTH; i++){
		alpha[i].shNodeID = i;
		alpha[i].shLQE = i;
		alpha[i].nodeID = i;
		alpha[i].networkID = i;
	}

	#ifdef LINUX
	printf("Alpha size = %d\n", sizeof(alpha));
	printf("Test size = %d\n", sizeof(i));
	#endif
}

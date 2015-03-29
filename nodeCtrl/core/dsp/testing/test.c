#include <stdlib.h>
#include "dsp.h"

#ifdef LINUX
	#include <stdio.h>
#endif

uint8_t i = 0;

void debug_print(uint8_t sample){
#ifdef LINUX
	printf("Sample %d: avg = %d, var = %d\n", i++, dsp_get_average(), dsp_get_variance());
//	printf("   Total = %d, variance = %d\n\n", dspStatus.sum, dspStatus.variance);
#endif
}

void main(){

	dsp_init(2, 0);
	debug_print(i);

	dsp_add_sample(10);
	debug_print(i);
	
	dsp_add_sample(5);
	debug_print(i);

	dsp_add_sample(5);
	debug_print(i);

	dsp_add_sample(5);
	debug_print(i);

	dsp_add_sample(5);
	debug_print(i);
	dsp_add_sample(5);
	debug_print(i);
	dsp_add_sample(5);
	debug_print(i);
	dsp_add_sample(5);
	debug_print(i);
}

#include <stdint.h>
#include "nodeCtrl.h"

void nodeCtrl_checksum(uint8_t *buf, uint8_t length)
{
	buf[length] = 0;
	int i;
	for(i=0; i++; i<length){
		buf[length] += buf[i];
	}		
}

void nodeCtrl_frame(uint8_t *buf, uint8_t length)
{
	/*nothing yet*/
}

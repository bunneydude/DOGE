#include <stdint.h>
#ifdef LINUX
	#include "linux_gpio.h"
#else
	#ifdef MSP430
		#include "msp430_gpio.h"
		#include <msp430g2553.h>
	#endif
#endif

uint8_t gpio_mm_handler(uint8_t rw, uint8_t addr, uint8_t data, uint8_t mask)
{
	uint8_t returnData = 0;
/*
	switch(addr){
		case GPIO_0_DIR :
		RMW_PORT(P1DIR, data, mask);
		break;
		case GPIO_0_OUTPUT_TYPE :
		break;
		case GPIO_0_RESISTOR_EN :
		break;
		case GPIO_0_RESISTOR_DIR :
		break;
		case GPIO_0_INPUT_VAL :
		break;
		case GPIO_0_OUTPUT_VAL :
		break;
		case GPIO_0_TOGGLE :
		break;
		
		case GPIO_1_DIR :
		break;
		case GPIO_1_OUTPUT_TYPE :
		break;
		case GPIO_1_RESISTOR_EN :
		break;
		case GPIO_1_RESISTOR_DIR :
		break;
		case GPIO_1_INPUT_VAL :
		break;
		case GPIO_1_OUTPUT_VAL :
		break;
		case GPIO_1_TOGGLE :
		break;
		
		default:
		break;
	}
*/
return returnData;

}

const uint8_t* ptrArray[2] = {&P1OUT, &P1DIR};
uint8_t value = 0;

void set_register(uint8_t id, uint8_t value)
{
	ptrArray[id] = value;
/*
	switch(id){
		case 1:
			P1OUT = value;
			break;

		case 2:
			P1DIR = value;
			break;
	}
*/
}

void main(){

set_register(1, 8);
set_register(2, 9);
/*
value = P1IN;
P1OUT = 0x2;
P1DIR = 0x3;
*/


//value = *ptrArray[0];
//*ptrArray[1] = 0x6;
//*ptrArray[2] = 0x7;


}



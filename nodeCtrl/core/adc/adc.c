#include <stdint.h>
#ifdef MSP430
#include <msp430g2553.h>
#endif

#include "../memory_map/memory_map.h"
#include "adc.h"


uint8_t adc_mm_handler(uint8_t rw, uint8_t addr, uint8_t* data, uint8_t mask)
{	
	uint16_t rawData = 0;
	if(addr < ADC_RO_MAX){ //in RO range
		if(addr >= ADC_RESULT_8){
			return UNSUPPORTED_FEATURE;
		}

		if(rw == 1){	
			switch(addr - ADC_RO_BAR){
				case ADC_RESULT_0:
					//rawData = analogRead(A0);
					break;	
				case ADC_RESULT_1:
					//rawData = analogRead(A1);
					break;	
				case ADC_RESULT_2:
					//rawData = analogRead(A2);
					break;	
				case ADC_RESULT_3:
					//rawData = analogRead(A3);
					break;	
				case ADC_RESULT_4:
					//rawData = analogRead(A4);
					break;	
				case ADC_RESULT_5:
					//rawData = analogRead(A5);
					break;	
				case ADC_RESULT_6:
					//rawData = analogRead(A6);
					break;	
				case ADC_RESULT_7:
					//rawData = analogRead(A7);
					break;	
				default:
					rawData = 0;
					break;
			}
			*data = (uint8_t)((rawData >> 2) & 0xFF);

		}else{ //error: trying to write a read-only register
			return RO_REGISTER;
		}

	}else if(addr < ADC_EMU_MAX){ //in EMU range
		if((~mask & 0x6) == 0x6){
			switch((*data & 0x6) >> 1){
				case 0:
					//analogReference(DEFAULT);
					break;
				case 1:
					//analogReference(INTERNAL1V5);
					break;
				case 2:
					//analogReference(INTERNAL2V5);
					break;
			}
		}else{
			return UNSUPPORTED_FEATURE;
		}
			
	}else{
		return ADDR_OUT_OF_RANGE;
	}

	return 0;
}





#include <stdlib.h>
#include <stdint.h>

#include "memory_map.h"

//#define mappedVariable *((&(memoryMap[1].u32)))
//static uint8_t* mappedArray = ((&(memoryMap[0].u8[2])));

uint8_t check_mm_space(uint8_t rw, uint8_t addr, uint8_t* data, uint8_t mask)
{
	if( addr < MM_FUNCTION_BAR ){
		return 1;
	}else if( (addr >= MM_GPIO_BASE) && (addr < (MM_GPIO_BASE + MM_GPIO_SIZE)) ){
		(*memoryMapRegionMethods.gpio_handler)(rw, addr, data, mask);
		return 2;

	}else if( (addr >= MM_ADC_BASE) && (addr < (MM_ADC_BASE + MM_ADC_SIZE)) ){
		(*memoryMapRegionMethods.adc_handler)(rw, addr, data, mask);
		return 3;

	}else if( (addr >= MM_UART_BASE) && (addr < (MM_UART_BASE + MM_UART_SIZE)) ){
		(*memoryMapRegionMethods.uart_handler)(rw, addr, data, mask);
		return 4;
	
	}else if( (addr >= MM_DSP_BASE) && (addr < (MM_DSP_BASE + MM_DSP_SIZE)) ){
		(*memoryMapRegionMethods.dsp_handler)(rw, addr, data, mask);
		return 5;

	}
	return 0;

}

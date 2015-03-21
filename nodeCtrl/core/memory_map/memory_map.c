#include <stdlib.h>
#include <stdint.h>

#include "memory_map.h"

#define mappedVariable *((&(memoryMap[1].u32)))
static uint8_t* mappedArray = ((&(memoryMap[0].u8[2])));

uint8_t check_mm_space(uint8_t addr, uint8_t data)
{
	if( addr < MM_FUNCTION_BAR ){
		return 1;
	}else if( (addr >= MM_GPIO_BASE) && (addr <= (MM_GPIO_BASE + MM_GPIO_SIZE-1)) ){
		(*memoryMapRegionMethods.gpio_handler)(1, addr, data);
		return 2;

	}else if( (addr >= MM_ADC_BASE) && (addr <= (MM_ADC_BASE + MM_ADC_SIZE-1)) ){
		(*memoryMapRegionMethods.adc_handler)(1, addr, data);
		return 3;

	}else if( (addr >= MM_UART_BASE) && (addr <= (MM_UART_BASE + MM_UART_SIZE-1)) ){
		(*memoryMapRegionMethods.uart_handler)(1, addr, data);
		return 4;
	
	}else if( (addr >= MM_DSP_BASE) && (addr <= (MM_DSP_BASE + MM_DSP_SIZE-1)) ){
		(*memoryMapRegionMethods.dsp_handler)(1, addr, data);
		return 4;

	}
	return 0;

}

#include <stdlib.h>
#include <stdint.h>

#include "memory_map.h"

#define mappedVariable *((&(memoryMap[1].u32)))
static uint8_t* mappedArray = ((&(memoryMap[0].u8[2])));

uint8_t check_mm_space(uint8_t addr)
{

	if( (addr >= MM_GPIO_BASE) && (addr <= (MM_GPIO_BASE + MM_GPIO_SIZE)) ){
		return 1;
	}else if( (addr >= MM_ADC_BASE) && (addr <= (MM_ADC_BASE + MM_ADC_SIZE)) ){
		return 2;
	}
	return 0;

}

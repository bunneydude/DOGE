#include <stdlib.h>
#include <stdint.h>

#ifdef LINUX
#include <stdio.h>
#endif

#include "memory_map.h"

#define mappedVariable *((&(memoryMap[1].u32)))

static uint8_t* mappedArray = ((&(memoryMap[0].u8[2])));

#ifdef LINUX
void linux_debug_print()
{
	printf("&memoryMap[0].u32 = %p\n", &(memoryMap[0].u32) );

	printf("&memoryMap[0].u16[0] = %p\n", &(memoryMap[0].u16[0]) );
	printf("&memoryMap[0].u16[1] = %p\n", &(memoryMap[0].u16[1]) );

	printf("&memoryMap[0].u8[0] = %p\n", &(memoryMap[0].u8[0]) );
	printf("&memoryMap[0].u8[1] = %p\n", &(memoryMap[0].u8[1]) );
	printf("&memoryMap[0].u8[2] = %p\n", &(memoryMap[0].u8[2]) );
	printf("&memoryMap[0].u8[3] = %p\n", &(memoryMap[0].u8[3]) );

	printf("&memoryMap[1].u32 = %p\n\n", &(memoryMap[1].u32) );


	memoryMap[0].u32 = 0xdeadbeef;
	printf("memoryMap[0].u32 = 0x%x\n", (memoryMap[0].u32) );

	memoryMap[0].u8[0] = 1;
	memoryMap[0].u8[1] = 2;
	printf("memoryMap[0].u32 = 0x%x\n", (memoryMap[0].u32) );
	printf("memoryMap[1].u32 = 0x%x\n", (memoryMap[1].u32) );


	mappedArray[0] = 4;
	mappedArray[1] = 5;
	mappedVariable = 6;

	printf("memoryMap[0].u32 = 0x%x\n", (memoryMap[0].u32) );
	printf("memoryMap[1].u32 = 0x%x\n", (memoryMap[1].u32) );


}
#endif


uint8_t gpio_print(uint8_t rw, uint8_t addr, uint8_t data)
{
#ifdef LINUX
	printf("In gpio_print. rw = 0x%x, address = 0x%x, data = 0x%x\n", rw, addr, data);
#endif
	return 0;
}

uint8_t adc_print(uint8_t rw, uint8_t addr, uint8_t data)
{
#ifdef LINUX
	printf("In adc_print. rw = 0x%x, address = 0x%x, data = 0x%x\n", rw, addr, data);
#endif
	return 0;
}

uint8_t uart_print(uint8_t rw, uint8_t addr, uint8_t data)
{
#ifdef LINUX
	printf("In uart_print. rw = 0x%x, address = 0x%x, data = 0x%x\n", rw, addr, data);
#endif
	return 0;
}

uint8_t dsp_print(uint8_t rw, uint8_t addr, uint8_t data)
{
#ifdef LINUX
	printf("In dsp_print. rw = 0x%x, address = 0x%x, data = 0x%x\n", rw, addr, data);
#endif
	return 0;
}

struct mmMethods memoryMapRegionMethods;

void main(){

memoryMapRegionMethods.gpio_handler = gpio_print;
memoryMapRegionMethods.adc_handler = adc_print;
memoryMapRegionMethods.uart_handler = uart_print;
memoryMapRegionMethods.dsp_handler = dsp_print;


#ifdef LINUX
	//linux_debug_print();
#endif

	uint8_t inputAddress = 0x10;
	uint8_t result = 0;
	uint8_t inputData = 10;
	
	for(inputAddress = 0x0; inputAddress < 0x20; inputAddress++){
		
		result = check_mm_space(inputAddress, inputData++);
#ifdef LINUX
		printf("Address 0x%x is in memory space %d\n\n", inputAddress, result);
#endif
	}

}


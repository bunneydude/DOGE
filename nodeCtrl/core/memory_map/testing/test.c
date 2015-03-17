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

void main(){
#ifdef LINUX
	//linux_debug_print();
#endif

	uint8_t inputAddress = 0x10;
	uint8_t result = 0;
	result = check_mm_space(inputAddress);

	#ifdef LINUX
	printf("Address 0x%x is in memory space %d\n", inputAddress, result);
	#endif

}


#include <stdio.h>
#include <stdint.h>

/*

rsvd - [7:7]
addr - [6:1]
flag - [0:0]

*/

#define ADDR_MASK 0x3f
#define ADDR_SHIFT 0x1

#define FLAG_MASK 0x1
#define FLAG_SHIFT 0x0

uint8_t set_addr(uint8_t addr)
{
	return ((addr & ADDR_MASK) << ADDR_SHIFT);
}

int main(int argc, char *argv[])
{
	uint8_t value = set_addr(0x3);
	printf("Value = %d\n", value);

	return 0;
}

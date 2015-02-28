#include <stdio.h>
#include <stdint.h>

/*

rsvd - [7:7]
addr - [6:1]
flag - [0:0]

*/
enum fields{
	FIELD_ADDR = 0,
	FIELD_FLAG
};

static uint8_t maskList[2] = {0x3f, //addrMask
			      0x01}; //fieldMask

static uint8_t shiftList[2] = {0x01, //addrShift
			      0x00}; //fieldShift



static uint8_t set_addr(uint8_t addr)
{
	return ((addr & maskList[FIELD_ADDR]) << shiftList[FIELD_ADDR]);
}
/*
uint8_t set_flag(uint8_t flag)
{
	return ((flag & maskList[FIELD_FLAG]) << shiftList[FIELD_FLAG]);
}
*/

int main(int argc, char *argv[])
{
	uint8_t value = set_addr(argc);
	printf("addr = %d\n", value);
	
//	value = set_flag(argc);
//	printf("flag = %d\n", value);
	return 0;
}

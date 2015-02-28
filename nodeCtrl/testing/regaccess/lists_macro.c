#include <stdio.h>
#include <stdint.h>

/*

rsvd - [7:7]
addr - [6:1]
flag - [0:0]

*/

enum fieldList{
	FIELD_ADDR = 0,
	FIELD_FLAG = 1
};

static uint8_t maskList[2] = {0x3f, //addrMask
			      0x01}; //fieldMask

static uint8_t shiftList[2] = {0x01, //addrShift
			      0x00}; //fieldShift

#define SET_FIELD(id, val) ((val & maskList[id])<<shiftList[id])

/*
uint8_t set_addr(uint8_t addr)
{
	return SET_FIELD(FIELD_ADDR, addr);
}
*/
/*
uint8_t set_flag(uint8_t flag)
{
	return SET_FIELD(FIELD_FLAG, flag);
}
*/
int main(int argc, char *argv[])
{
	uint8_t value = SET_FIELD(FIELD_ADDR, argc);
	printf("addr = %d\n", value);
	
//	value = set_flag(argc);
//	printf("flag = %d\n", value);
	return 0;
}

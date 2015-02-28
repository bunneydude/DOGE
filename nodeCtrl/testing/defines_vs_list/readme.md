This subfolder is testing two ways of writing the code for the nRF24l01p radio. The first method, 'defines', uses the (most likely) more familiar method of explicitly defining a MASK and SHIFT value for each field. The second method, 'list', uses a static array for the MASK and SHIFT values. This allows for a static function to (likely) be inlined by the compiler. 

When assigning to a field within a register, it is a good idea to use masks and shifts to prevent writing to a different register field. The 'defines' approach would look like:

#define SHIFT_VALUE_A 1
#define SHIFT_VALUE_B 3 

#define MASK_VALUE_A 0x6
#define MASK_VALUE_B 0x8

uint8_t valueA, valueB;

someRegister = ((valueA << SHIFT_VALUE_A) & MASK_VALUE_A) | ((valueB << SHIFT_VALUE_B) & MASK_VALUE_B);


The 'list' approach instead creates two arrays of shifts and masks, an enum for indexing into the lists, and a function for performing the shift and bitwise AND:

static uint8_t shiftList[] = {
	1, //value_a
	3, //value_b
};

static uint8_t maskList[] = {
	0x6, //value_a
	0x8, //value_b
};

enum fields{
	VALUE_A = 0,
	VALUE_B = 1,
};	

uint8_t set_field(uint8_t id, uint8_t value)
{
	return ((value << shiftList[id[) & maskList[id]);
}

uint8_t valueA, valueB;

someRegister = set_field(VALUE_A, valueA) | set_field(VALUE_B, valueB);



To compare these methods I'm using the nrf24 library written by Kehribar (original license in nrf24.c and nrf24.h) as a base. Each subfolder has a basic makefile with an MSP430 target. 

Currently, the list method is smaller by 2 bytes of ROM. The static arrays were properly optimized out and it looks like msp430-gcc was able to use the return type of the 'set_field' function to omit clearing the top byte of a variable (in lastMessageStatus).



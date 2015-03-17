#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H


union memoryMapEntry{
	uint32_t u32;
	uint16_t u16[2];
	uint8_t u8[4];
};

union memoryMapEntry memoryMap[2]; 

enum mm_bases{
	MM_GPIO_BASE = 0x0,
	MM_ADC_BASE = 0x10,
};

enum mm_base_sizes{ //number of bytes needed, for now round up to next power of 2
	MM_GPIO_SIZE = 0xF,
	MM_ADC_SIZE = 0xF,
};


uint8_t check_mm_space(uint8_t addr);
	
//GPIO
//6 bytes of registers needed for every 8 pins

//MSP430G2553 has 16 pins -> needs 2*6=12 bytes in memory map -> round to 16
//LPC812 has 18 pins -> needs 3*6=18 bytes in memory map -> round to 24
enum mm_gpio_registers{
	GPIO_DIR = 0x1, //Direction: input, output
	GPIO_OUTPUT_TYPE = 0x2, //Output type: pushpull, open-drain
	GPIO_RESISTOR_EN = 0x3, //Pull resistor enable
	GPIO_RESISTOR_DIR = 0x4, //pull resistor direction
	GPIO_INPUT_VAL = 0x5, //input value
	GPIO_OUTPUT_VAL = 0x6, //output value
};

//collapse RES, UPPER_REF, LOWER_REF, and SAMPLE_TIME into one register

enum mm_adc_registers{
	ADC_RES = 0x1, //1 bit;   1 or 2 byte results
	ADC_UPPER_REF = 0x3, //2 bits;   at least 4 valid ones for MSP430
	ADC_LOWER_REF = 0x4, //1 bit;   gnd or external
	ADC_SAMPLE_TIME = 0x4, //2 bits;   for now, 4 options (b/c MSP uses 4, 8, 16, or 64 clks)	

	ADC_SPEED = 0x5, //value is interpreted as a division of the max ADC speed
	
	ADC_INPUT_VAL = 0x2,
};


#endif

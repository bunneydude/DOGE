#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H


union memoryMapEntry{
	uint32_t u32;
	uint16_t u16[2];
	uint8_t u8[4];
};


//method arguments are (read-write flag, address, data)
struct mmMethods{
	uint8_t (*gpio_handler)(uint8_t, uint8_t, uint8_t);
	uint8_t (*adc_handler)(uint8_t, uint8_t, uint8_t);
	uint8_t (*uart_handler)(uint8_t, uint8_t, uint8_t);
	uint8_t (*dsp_handler)(uint8_t, uint8_t, uint8_t);
} memoryMapRegionMethods;
	

enum mm_bases{
	MM_PHYSICAL_BAR = 0x0, //actual memory
	MM_NETWORK_BASE = 0x0,

	MM_FUNCTION_BAR = 0x8, //start of memory-mapped functions
	MM_GPIO_BASE = 0x8,
	MM_ADC_BASE = 0x18,
	MM_UART_BASE = 0x1C,
	MM_DSP_BASE = 0x20,
};

enum mm_base_sizes{ //number of bytes needed, for now round up to next power of 2
	MM_NETWORK_SIZE = 8,
	
	MM_GPIO_SIZE = 0x10,
	MM_ADC_SIZE = 0x4,
	MM_UART_SIZE = 0x4,
	MM_DSP_SIZE = 0x4
};

union memoryMapEntry memoryMap[MM_NETWORK_SIZE]; 

//mm region is [base, base + size -1]

uint8_t check_mm_space(uint8_t addr, uint8_t data);
	
//GPIO
//6 bytes of registers needed for every 8 pins

//MSP430G2553 has 16 pins -> needs 2*6=12 bytes in memory map -> round to 16
//LPC812 has 18 pins -> needs 3*6=18 bytes in memory map -> round to 24
enum mm_gpio_registers{
	GPIO_RO_BAR      = 0x0,
	GPIO_0_INPUT_VAL = 0x0, //input value
	GPIO_1_INPUT_VAL = 0x1, //input value
	GPIO_RO_MAX      = 0x2, 

	GPIO_RW_BAR         = 0x2,
	GPIO_0_OUTPUT_VAL   = 0x2, //output value
	GPIO_0_RESISTOR_DIR = 0x2, //pull resistor direction //Special - on MSP430, the output reg control pull resistor direction
	GPIO_0_DIR          = 0x3, //Direction: input, output
	GPIO_0_RESISTOR_EN  = 0x4, //Pull resistor enable
	GPIO_1_OUTPUT_VAL   = 0x5, //output value
	GPIO_1_RESISTOR_DIR = 0x5, //pull resistor direction //Special - on MSP430, the output reg control pull resistor direction
	GPIO_1_DIR          = 0x6, //Direction: input, output
	GPIO_1_RESISTOR_EN  = 0x7, //Pull resistor enable
	GPIO_RW_MAX         = 0x8,

	GPIO_EMU_BAR       = 0x8,	
	GPIO_0_OUTPUT_TYPE = 0x8, //Output type: pushpull, open-drain
	GPIO_0_TOGGLE      = 0x9,
	GPIO_1_OUTPUT_TYPE = 0xa, //Output type: pushpull, open-drain
	GPIO_1_TOGGLE      = 0xb,
	GPIO_EMU_MAX       = 0xc,
};

//collapse RES, UPPER_REF, LOWER_REF, and SAMPLE_TIME into one register

enum mm_adc_registers{
//	ADC_RES = 0x0, //1 bit;   1 or 2 byte results
//	ADC_UPPER_REF = 0x0, //2 bits;   at least 4 valid ones for MSP430
//	ADC_LOWER_REF = 0x0, //1 bit;   gnd or external
//	ADC_SAMPLE_TIME = 0x0, //2 bits;   for now, 4 options (b/c MSP uses 4, 8, 16, or 64 clks)	
	ADC_CFG0 = 0x0,	
	ADC_SPEED = 0x1, //value is interpreted as a division of the max ADC speed	
	ADC_INPUT_VAL = 0x2,
};

enum mm_uart_registers{
//	UART_DATA_BITS = 0x0, //3bits - 5,6,7,8,9 data bits
//	UART_STOP_BITS = 0x0, //1 bit - 1, 2 bits
//	UART_PARITY = 0x0, //2 bits - even, odd, none
//	UART_ORDER = 0x0, //1 bit - MSB or LSB-first

	UART_CFG0 = 0x0,
	UART_TX = 0x1, //write to this register to transmit TODO does 1byte cause an issue for 9-bit data?
	UART_RX = 0x2, //read from this register to get received data (no mask needed)
};

enum mm_dsp_registers{
	DSP_RUN_LENGTH = 0x0, //1-255
	DSP_AVERAGE = 0x1,
	DSP_VARIANCE = 0x2,
};

#endif

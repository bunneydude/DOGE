GPIO_RO_BAR      = 0x0
GPIO_0_INPUT_VAL = 0x0 #//input value
GPIO_1_INPUT_VAL = 0x1 #//input value
GPIO_RO_MAX      = 0x2 

GPIO_RW_BAR         = 0x2
GPIO_0_OUTPUT_VAL   = 0x2 #//output value
GPIO_0_RESISTOR_DIR = 0x2 #//pull resistor direction //Special - on MSP430 the output reg control pull resistor direction
GPIO_0_DIR          = 0x3 #//Direction: input output
GPIO_0_RESISTOR_EN  = 0x4 #//Pull resistor enable
GPIO_1_OUTPUT_VAL   = 0x5 #//output value
GPIO_1_RESISTOR_DIR = 0x5 #//pull resistor direction //Special - on MSP430 the output reg control pull resistor direction
GPIO_1_DIR          = 0x6 #//Direction: input output
GPIO_1_RESISTOR_EN  = 0x7 #//Pull resistor enable
GPIO_RW_MAX         = 0x8

GPIO_EMU_BAR       = 0x8	
GPIO_0_OUTPUT_TYPE = 0x8 #//Output type: pushpull open-drain
GPIO_0_TOGGLE      = 0x9
GPIO_1_OUTPUT_TYPE = 0xa #//Output type: pushpull open-drain
GPIO_1_TOGGLE      = 0xb
GPIO_EMU_MAX       = 0xc

########################


ADC_RO_BAR      = 0x0
ADC_RESULT_0    = 0x0
ADC_RESULT_1    = 0x1
ADC_RESULT_2    = 0x2
ADC_RESULT_3    = 0x3
ADC_RESULT_4    = 0x4
ADC_RESULT_5    = 0x5
ADC_RESULT_6    = 0x6
ADC_RESULT_7    = 0x7
ADC_RESULT_8    = 0x8
ADC_RESULT_9    = 0x9
ADC_RESULT_10   = 0xa
ADC_RESULT_11   = 0xb
ADC_RO_MAX      = 0xc

ADC_EMU_BAR     = 0xc
ADC_CONFIG      = 0xd
#//		ADC_RES         = 0x1 //1 bit;  1 or 2 byte results
#//		ADC_UPPER_REF   = 0x2 //2 bits: at least 4 valid ones for MSP430
#//		ADC_LOWER_REF   = 0x3 //1 bit:  gnd or external
#//		ADC_SAMPLE_TIME = 0x4 //2 bits; for now 4 options (b/c MSP uses 4, 8, 16, or 64 clks)	
ADC_SPEED       = 0xe #//8 bits; value is interpreted as a division of the max ADC speed	
ADC_EMU_MAX     = 0xf

#######

UART_RO_BAR    = 0x0
UART_RX        = 0x0 #//read from this register to get received data (no mask needed)
UART_RO_MAX    = 0x1

UART_RW_BAR    = 0x1
UART_TX        = 0x1 #//write to this register to transmit TODO does 1byte cause an issue for 9-bit data?
UART_RW_MAX    = 0x2

UART_EMU_BAR   = 0x2
UART_CONFIG    = 0x2
#//		UART_DATA_BITS = 0x2 //3bits - 5,6,7,8,9 data bits
#//		UART_STOP_BITS = 0x3 //1 bit - 1, 2 bits
#//		UART_PARITY    = 0x4 //2 bits - even, odd, none
#//		UART_ORDER     = 0x5 //1 bit - MSB or LSB-first
UART_EMU_MAX   = 0x3

#################

DSP_EMU_BAR    = 0x0
DSP_AVERAGE    = 0x0
DSP_VARIANCE   = 0x1
DSP_RUN_LENGTH = 0x2 #//1-255, power of 2
DSP_CHANNEL    = 0x3
DSP_PERIOD     = 0x4
DSP_EMU_MAX    = 0x8

############

ERROR_PROTOCOL_BASE = 0x0  #//7
ERROR_GPIO_BASE     = 0x8  #//4
ERROR_ADC_BASE      = 0xC  #//4
ERROR_RADIO_BASE    = 0x10 #// 2-4?
ERROR_MEMORY_MAP_BASE = 0x14 #//4?

RO_REGISTER         = ERROR_MEMORY_MAP_BASE + 0x0
WO_REGISTER         = ERROR_MEMORY_MAP_BASE + 0x1
ADDR_OUT_OF_RANGE   = ERROR_MEMORY_MAP_BASE + 0x2
UNSUPPORTED_FEATURE = ERROR_MEMORY_MAP_BASE + 0x3

#STATIC MM ADDRESSES FOR NRF24 NODES
NRF24_NODE_11 = 0x0
NRF24_NODE_12 = 0x1
NRF24_NODE_13 = 0x2
NRF24_NODE_14 = 0x3
NRF24_NODE_15 = 0x4
NRF24_NODE_16 = 0x5
NRF24_NODE_17 = 0x6
NRF24_NODE_18 = 0x7

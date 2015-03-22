#ifndef ADC_H
#define ADC_H

uint8_t adc_mm_handler(uint8_t rw, uint8_t addr, uint8_t* data, uint8_t mask);


enum adcErrorCodes{
	RO_REGISTER         = 0x1,
	ADDR_OUT_OF_RANGE   = 0x2,
	UNSUPPORTED_FEATURE = 0x3,
};

#endif

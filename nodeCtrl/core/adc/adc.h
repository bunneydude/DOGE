#ifndef ADC_H
#define ADC_H

#include "../nodeCtrl_errno.h"

uint8_t adc_mm_handler(uint8_t rw, uint8_t addr, uint8_t* data, uint8_t mask);


enum adcErrorCodes{
	RO_REGISTER         = ERROR_ADC_BASE + 0x1,
	ADDR_OUT_OF_RANGE   = ERROR_ADC_BASE + 0x2,
	UNSUPPORTED_FEATURE = ERROR_ADC_BASE + 0x3
};

#endif

#ifndef ADC_H
#define ADC_H

#include "../nodeCtrl_errno.h"

uint8_t adc_mm_handler(uint8_t rw, uint8_t addr, uint8_t* data, uint8_t mask);

#endif

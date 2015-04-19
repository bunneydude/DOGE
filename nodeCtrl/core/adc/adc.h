#ifndef ADC_H
#define ADC_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "../nodeCtrl_errno.h"

uint8_t adc_mm_handler(uint8_t rw, uint8_t addr, uint8_t* data, uint8_t mask);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

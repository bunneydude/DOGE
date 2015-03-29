#ifndef DSP_H
#define DSP_H

#include <stdint.h>

struct dspStatus{
	uint16_t sum;
	uint16_t variance;
	uint8_t runLength;
	uint8_t sampleCount;
	uint8_t channel;
};

struct dspStatus dspStatus;

void dsp_add_sample(uint16_t data);
void dsp_init(uint8_t runLength, uint8_t channel);
uint8_t dsp_get_average();
uint8_t dsp_get_variance();


#endif

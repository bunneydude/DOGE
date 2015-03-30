#ifndef DSP_H
#define DSP_H

#include <stdint.h>
#include "../memory_map/memory_map.h"

struct dspStatus{
	uint16_t sum;
	uint16_t variance;
	uint8_t runLength;
	uint8_t sampleCount;
	uint8_t channel;
	uint8_t period; //resolution of seconds
	uint8_t counter;
};

struct dspStatus dspStatus;

void dsp_add_sample(uint16_t data);
void dsp_init(uint8_t runLength, uint8_t channel);
uint8_t dsp_get_average();
uint8_t dsp_get_variance();

uint8_t dsp_mm_handler(uint8_t rw, uint8_t addr, uint8_t* data, uint8_t mask);

#endif

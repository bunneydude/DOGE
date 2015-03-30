/*
This file implements a running average and variance without
requiring a buffer of N samples (where N is the window size).

The code was based off the following blog post and git repo
by Scott Daniels:
http://provideyourown.com/2012/statistics-on-the-arduino/
https://github.com/provideyourown/statistics

While an efficient running average of this kind is not novel
(c.f. http://www.daycounter.com/LabBook/Moving-Average.phtml),
Scott's walkthrough of the approach was useful and deserves credit.
*/

#include <stdint.h>
#include "dsp.h"

void dsp_init(uint8_t runLength, uint8_t channel)
{
	dspStatus.runLength = runLength;
	dspStatus.sampleCount = 0;
	dspStatus.sum = 0;
	dspStatus.variance = 0;
	dspStatus.channel = channel;
	dspStatus.period = 0;
	dspStatus.counter = 0;
}

void dsp_add_sample(uint16_t data)
{

	if(dspStatus.sampleCount >= dspStatus.runLength){
		dspStatus.sum = (dspStatus.sum*(dspStatus.runLength - 1)) /dspStatus.runLength;
		dspStatus.variance = (dspStatus.variance*(dspStatus.runLength - 1)) /dspStatus.runLength;
	}else{
		dspStatus.sampleCount++;
	}

	dspStatus.sum += data;
	dspStatus.variance += data*data;
}

uint8_t dsp_get_average()
{
	if(dspStatus.sampleCount > 0){
		return (uint8_t)(dspStatus.sum/dspStatus.sampleCount);
	}else{
		return 0;
	}
}

uint8_t dsp_get_variance()
{
	if(dspStatus.sampleCount > 0){
		uint8_t avg = dsp_get_average();
		return (dspStatus.variance - (dspStatus.sampleCount * avg * avg)) / dspStatus.sampleCount;
	}else{
		return 0;
	}
}

uint8_t dsp_mm_handler(uint8_t rw, uint8_t addr, uint8_t* data, uint8_t mask){

	uint8_t returnData = 0;
	if(addr < DSP_EMU_MAX){ //in EMU range
		switch(addr){
			case DSP_AVERAGE:
				if(rw == 1){
					*data = dspStatus.sum;
				}else{
					dspStatus.sum = *data;
				}
				break;
			case DSP_VARIANCE:
				if(rw == 1){
					*data = dspStatus.variance;
				}else{
					dspStatus.variance = *data;
				}
				break;
			case DSP_RUN_LENGTH:
				if(rw == 1){
					*data = dspStatus.runLength;
				}else{
					dspStatus.runLength = *data;
				}
				break;
			case DSP_CHANNEL:
				if(rw == 1){
					*data = dspStatus.channel;
				}else{
					dspStatus.channel = *data;
				}
				break;
			case DSP_PERIOD:
				if(rw == 1){
					*data = dspStatus.period;
				}else{
					dspStatus.period = *data;
				}
				break;
			default:
				return UNSUPPORTED_FEATURE;
				break;
		}
	}else{
		return ADDR_OUT_OF_RANGE;
	}
	return 0;
}

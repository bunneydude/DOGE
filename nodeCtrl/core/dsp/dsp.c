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

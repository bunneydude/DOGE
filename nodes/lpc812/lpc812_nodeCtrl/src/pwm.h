/*
 * pwm.h
 *
 *  Created on: Nov 16, 2014
 *      Author: bunney
 */

#ifndef PWM_H_
#define PWM_H_

#include "LPC8xx.h"

#define SCT 8
#define SCT_RST_N 8
#define UNIFY 0


void pwm_init();
void pwm_set_channel(uint8_t channel, uint8_t pin);
void pwm_write(uint8_t channel, uint8_t output);

#endif /* PWM_H_ */

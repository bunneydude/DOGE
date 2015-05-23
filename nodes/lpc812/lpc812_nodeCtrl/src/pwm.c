/*
 * pwm.c
 *
 *  Created on: Nov 16, 2014
 *      Author: bunney
 */

#include "pwm.h"

// Saved tick count used for a PWM cycle
static uint32_t cycleTicks;

// PWM cycle time - time of a single PWM sweep
#define PWMCYCLERATE (1000)

void pwm_init(){


	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<SCT); // enable SCT
	LPC_SYSCON->PRESETCTRL |= (1<<SCT_RST_N); // reset SCT

	LPC_SCT->CONFIG = (1<<UNIFY); //SCT_CONFIG_32BIT_COUNTER | SCT_CONFIG_CLKMODE_BUSCLK;

	// Initial CTOUT0 state is high
	LPC_SCT->OUTPUT = (7 << 0);

	cycleTicks = __SYSTEM_CLOCK / PWMCYCLERATE; //1ms ticks

	// Setup for match mode
	LPC_SCT->REGMODE_L = 0;


	// Setup match counter 0 for the number of ticks in a PWM sweep, event 0
	// will be used with the match 0 count to reset the counter.
	LPC_SCT->MATCH[0].U = cycleTicks;
	LPC_SCT->MATCHREL[0].U = cycleTicks;
	LPC_SCT->EVENT[0].CTRL = 0x00001000;
	LPC_SCT->EVENT[0].STATE = 0xFFFFFFFF;
	LPC_SCT->LIMIT_L = (1 << 0);

	// For CTOUT0 to CTOUT2, event 1 is used to clear the output
	LPC_SCT->OUT[0].CLR = (1 << 0);
	LPC_SCT->OUT[1].CLR = (1 << 0);
	LPC_SCT->OUT[2].CLR = (1 << 0);

	//init to 50%
	pwm_write(0, 50);
	pwm_write(1, 50);
	pwm_write(2, 50);

	// Setup event 1 to trigger on match 1 and set CTOUT0 high
	LPC_SCT->EVENT[1].CTRL = (1 << 0) | (1 << 12);
	LPC_SCT->EVENT[1].STATE = 1;
	LPC_SCT->OUT[0].SET = (1 << 1);

	// Setup event 2 trigger on match 2 and set CTOUT1 high
	LPC_SCT->EVENT[2].CTRL = (2 << 0) | (1 << 12);
	LPC_SCT->EVENT[2].STATE = 1;
	LPC_SCT->OUT[1].SET = (1 << 2);

	// Setup event 3 trigger on match 3 and set CTOUT2 high
	LPC_SCT->EVENT[3].CTRL = (3 << 0) | (1 << 12);
	LPC_SCT->EVENT[3].STATE = 1;
	LPC_SCT->OUT[2].SET = (1 << 3);


	// Don't use states
	LPC_SCT->STATE_L = 0;

	// Unhalt the counter to start
	LPC_SCT->CTRL_U &= ~(1 << 2);
}

void pwm_set_channel(uint8_t channel, uint8_t pin){
	switch(channel) {
	case 0: // CTOUT_0
		LPC_SWM->PINASSIGN6 = (LPC_SWM->PINASSIGN6 & 0x00ffffff) | pin<<24;
		break;
	case 1: // CTOUT_1
		LPC_SWM->PINASSIGN7 = (LPC_SWM->PINASSIGN7 & 0xffffff00) | pin<<0;
		break;
	case 2: // CTOUT_2
		LPC_SWM->PINASSIGN7 = (LPC_SWM->PINASSIGN7 & 0xffff00ff) | pin<<8;
		break;
	case 3: // CTOUT_3
		LPC_SWM->PINASSIGN7 = (LPC_SWM->PINASSIGN7 & 0xff00ffff) | pin<<16;
		break;
	}
}

void pwm_write(uint8_t channel, uint8_t output){
	uint32_t value;

	/* Limit valid PWMs to 3 */
	if ((channel < 0) || (channel > 3)) {
		return;
	}

	if (output >= 100) {
		value = 1;
	}
	else if (output == 0) {
		value = cycleTicks + 1;
	}
	else {
		uint32_t newTicks;

		//newTicks = cycleTicks * ((output/255.0) * 100);
		newTicks = ((cycleTicks * output) / 100);
		/* Approximate duty cycle rate */
		value = cycleTicks - newTicks;
	}

	LPC_SCT->MATCHREL[channel + 1].U = value;

}



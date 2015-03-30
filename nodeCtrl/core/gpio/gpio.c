#include <stdint.h>
#ifdef LINUX
	#include "linux_gpio.h"
#else
	#ifdef MSP430
		#include "msp430_gpio.h"
		#include <msp430g2553.h>
	#endif
#endif
#include "../memory_map/memory_map.h"
#include "gpio.h"

static const volatile uint8_t* gpioRORegisters[2] = {&P1IN, &P2IN};
static volatile uint8_t* gpioRWRegisters[6] = {&P1OUT, &P1DIR, &P1REN, &P2OUT, &P2DIR, &P2REN};

// addr is in the range [0, 0xb] - i.e. the GPIO base address has already been removed
// The first step is to determine if this address is in the RO, RW, or EMU region
uint8_t gpio_mm_handler(uint8_t rw, uint8_t addr, uint8_t* data, uint8_t mask)
{
	uint8_t returnData = 0;
	if(addr < GPIO_RO_MAX){ //in RO range
		if(rw == 1){
			R_REG(*gpioRORegisters[addr - GPIO_RO_BAR], *data, mask);
		}else{ //error: trying to write a read-only register
			return RO_REGISTER;
		}

	}else if(addr < GPIO_RW_MAX){ //in RW range
		if(rw == 1){
			R_REG(*gpioRWRegisters[addr - GPIO_RW_BAR], *data, mask);
		}else{
			RMW_REG(*gpioRWRegisters[addr - GPIO_RW_BAR], *data, mask);
		}
			
	}else if(addr < GPIO_EMU_MAX){ //in EMU range
		if(rw == 1){ //all emulated gpio registers are write-only
			return WO_REGISTER;
		}

		switch(addr){
			case GPIO_0_TOGGLE:				
				P1OUT ^= *data & ~mask;
				break;
			case GPIO_1_TOGGLE:
				P2OUT ^= *data & ~mask;
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


void main(){


}



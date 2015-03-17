#include <stdio.h>
#include <stdint.h>

#include "nrf24l01p-const.h"
#ifdef LINUX
#include "platform-edison.h"
#else
	#ifdef MSP430
	#include "platform-msp430.h"
	#else
	#pragma error("No target defined with -D")
	#endif
#endif

int main(int argc, char *argv[])
{
//	printf("Hello world, %d\n", NRF24L01P_FIELD_EN_CRC);
//	nrf24_init();
//	nrf24_set_channel(2);
//	nrf24_set_channel(3);
	nrf24_ce_digitalWrite(0);
	NRF24_CE_SET();
	return 0;
}

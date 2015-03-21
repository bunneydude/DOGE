#include <stdint.h>
#include <stdio.h>

#include "platform-edison.h"

void nrf24_ce_digitalWrite(uint8_t state)
{
	printf("Test log: NRF CE write = [%d]\n", state);
	return;
}
 
void nrf24_csn_digitalWrite(uint8_t state)
{
	printf("Test log: NRF CSN write = [%d]\n", state);
	return;
}

uint8_t spi_transfer(uint8_t tx, uint8_t send_eot)
{
	printf("Test log: SPI, EOT = [%d, %d]\n", tx, send_eot);
}


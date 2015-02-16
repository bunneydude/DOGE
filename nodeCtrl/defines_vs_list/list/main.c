#include <stdio.h>
#include <stdint.h>

#include "nrf24l01p-const-enums.h"



void nrf24_ce_digitalWrite(uint8_t state)
{
//	printf("Test log: NRF CE write = [%d]\n", state);
	return;
}
 
void nrf24_csn_digitalWrite(uint8_t state)
{
//	printf("Test log: NRF CSN write = [%d]\n", state);
	return;
}

uint8_t spi_transfer(uint8_t tx, uint8_t send_eot)
{
//	printf("Test log: SPI, EOT = [%d, %d]\n", tx, send_eot);
}

int main(int argc, char *argv[])
{
//	printf("Hello world, %d\n", NRF24L01P_FIELD_EN_CRC);
	nrf24_init();
	nrf24_set_channel(2);
	nrf24_set_channel(3);
	return 0;
}

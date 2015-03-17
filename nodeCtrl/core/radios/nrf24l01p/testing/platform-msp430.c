#include <stdint.h>
#include "platform-msp430.h"

void nrf24_ce_digitalWrite(uint8_t state)
{
	if(state == 0){
		POUT_NRF24L01P_CE &= ~NRF24L01P_CE_BIT;
	}else{
		POUT_NRF24L01P_CE |= NRF24L01P_CE_BIT;
	}
}
 
void nrf24_csn_digitalWrite(uint8_t state)
{
	if(state == 0){
		POUT_NRF24L01P_CSN &= ~NRF24L01P_CSN_BIT;
	}else{
		POUT_NRF24L01P_CSN |= NRF24L01P_CSN_BIT;
	}
}

void nrf24_ce_set()
{
	POUT_NRF24L01P_CE |= NRF24L01P_CE_BIT;
}


void nfr24_ce_clear()
{
	POUT_NRF24L01P_CE &= ~NRF24L01P_CE_BIT;
}

uint8_t spi_transfer(uint8_t tx, uint8_t send_eot)
{
}


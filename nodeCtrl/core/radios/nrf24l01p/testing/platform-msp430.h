#ifndef PLATFORM_MSP430_H_
#define PLATFORM_MSP430_H_

#include <msp430g2553.h>


#define POUT_NRF24L01P_CE P1OUT
#define POUT_NRF24L01P_CSN P1OUT

#define NRF24L01P_CSN_BIT 0x10
#define NRF24L01P_CE_BIT 0x08
#define NRF24_CE_SET() (POUT_NRF24L01P_CE |= NRF24L01P_CE_BIT)


//void nrf24_ce_digitalWrite(uint8_t state);
//void nrf24_csn_digitalWrite(uint8_t state);
//uint8_t spi_transfer(uint8_t tx, uint8_t send_eot);
void nrf24_ce_set(); 
void nrf24_ce_clear();
#endif

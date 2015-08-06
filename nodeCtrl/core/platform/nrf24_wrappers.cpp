#include "platform.h"

//functions for nRF
void nrf24_ce_digitalWrite(uint8_t state){
  digital_write(RADIO_NRF_CE, state);
}

void nrf24_csn_digitalWrite(uint8_t state){
  digital_write(RADIO_NRF_CSN, state);
}

#ifdef __LPC8XX__
uint8_t spi_transfer(uint8_t tx, uint8_t send_eot)
{
   spiTransfer(LPC_SPI0, tx, send_eot);
   return LPC_SPI0->RXDAT;
}
#elif defined(MSP430)
uint8_t spi_transfer(uint8_t tx, uint8_t send_eot)
{
  uint8_t rx = 0;    
  rx = SPI.transfer(tx);
  return rx;
}
#endif
/*
void print_nrf_regs(){
   uint8_t temp=0;
   nrf24_readRegister(RF_CH, &temp,1);
   printf("RF_CH: %d\n\r", temp);

   nrf24_readRegister(RX_PW_P0, &temp,1);
   printf("RX_PW_P0: %d\n\r", temp);

   nrf24_readRegister(RX_PW_P1, &temp,1);
   printf("RX_PW_P1: %d\n\r", temp);

   nrf24_readRegister(RX_PW_P2, &temp, 1);
   printf("RX_PW_P2: %d\n\r", temp);

   nrf24_readRegister(RX_PW_P3, &temp, 1);
   printf("RX_PW_P3: %d\n\r", temp);

   nrf24_readRegister(RX_PW_P4, &temp, 1);
   printf("RX_PW_P4: %d\n\r", temp);

   nrf24_readRegister(RX_PW_P5, &temp, 1);
   printf("RX_PW_P5: %d\n\r", temp);

   nrf24_readRegister(RF_SETUP, &temp, 1);
   printf("RF_SETUP: %d\n\r", temp);

   nrf24_readRegister(CONFIG, &temp, 1);
   printf("CONFIG: %d\n\r", temp);

   nrf24_readRegister(EN_AA, &temp, 1);
   printf("EN_AA: %d\n\r", temp);

   nrf24_readRegister(EN_RXADDR, &temp, 1);
   printf("EN_RXADDR: %d\n\r", temp);

   nrf24_readRegister(SETUP_RETR, &temp, 1);
   printf("SETUP_RETR: %d\n\r", temp);

   nrf24_readRegister(DYNPD, &temp, 1);
   printf("DYNPD: %d\n\r", temp);
}
*/


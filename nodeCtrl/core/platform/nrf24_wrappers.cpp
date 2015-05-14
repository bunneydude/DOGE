#include "../nrfLegacy/nrf24.h"
#include "../proxy_nodeCtrl.h"
#include <SPI.h>

//functions for nRF
void nrf24_ce_digitalWrite(uint8_t state){
  digitalWrite(RADIO_NRF_CE,state);
}

void nrf24_csn_digitalWrite(uint8_t state){
  digitalWrite(RADIO_NRF_CSN,state);
}

uint8_t spi_transfer(uint8_t tx, uint8_t send_eot)
{
  uint8_t rx = 0;    
  rx = SPI.transfer(tx);
  return rx;
}

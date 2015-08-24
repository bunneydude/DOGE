#include "doge_radio.h"

#ifdef __LPC8XX__
uint8_t sending()
{
   return nrf24_isSending();
}

uint8_t get_data(uint8_t* data, uint8_t length, uint16_t timeout)
{
   return nrf24_timeoutRead(data, length, timeout);
}

void send_data(uint8_t address, uint8_t* data, uint8_t length)
{
   nrf24_send(0, data, length);
}

int8_t get_rssi()
{
   return 0;
}
#endif

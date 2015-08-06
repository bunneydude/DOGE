#include "doge_radio.h"

#ifdef __LPC8XX__
uint8_t sending()
{
   return 0;
}

uint8_t get_data(uint8_t* data, uint8_t length, uint16_t timeout)
{
   return 0;
}

void send_data(uint8_t address, uint8_t* data, uint8_t length)
{
   return;
}

int8_t get_rssi()
{
   return 0;
}
#endif

#include "doge_radio.h"

unsigned char radio_receiver_on(uint8_t *dataField, uint8_t length, uint16_t timeout)
{
  Radio.receiverOn(dataField, length, timeout);
}

void radio_begin(uint8_t address, channel_t channel, power_t power)
{
   Radio.begin(address, channel, power);
}

void radio_transmit(uint8_t address, uint8_t *dataField, uint8_t length)
{
   Radio.transmit(address, dataField, length);
}

#ifndef RADIOS_H
#define RADIOS_H

struct radioMethods{
   uint8_t (*const sending)(void);
   uint8_t (*const get_data)(uint8_t*, uint8_t, uint16_t); //buffer, size, timeout
   void (*const send_data)(uint8_t, uint8_t*, uint8_t); //channel, buffer, size
   int8_t (*const get_rssi)(void);
};

extern struct radioMethods dogeRadios[];

#endif

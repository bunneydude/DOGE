#ifndef RADIOS_H
#define RADIOS_H

#define MAX_RADIO_NUMBER 4

struct radioMethods{
   uint8_t (*sending)(void);
   uint8_t (*get_data)(uint8_t*, uint8_t, uint16_t); //buffer, size, timeout
   uint8_t (*send_data)(uint8_t, uint8_t*, uint8_t); //channel, buffer, size
};

#define DOGE_BROADCAST 0 //todo...make this correct for other radios

extern struct radioMethods dogeRadios[MAX_RADIO_NUMBER];

#endif

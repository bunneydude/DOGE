#ifndef FRAMING_H
#define FRAMING_H

#include <stdint.h>

uint8_t cobs_encode(uint8_t* input, uint8_t* output, uint16_t size);
uint8_t cobs_decode(uint8_t* input, uint8_t* output, uint16_t size);


#endif

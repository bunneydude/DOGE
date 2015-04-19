#ifndef TEST_H
#define TEST_H

#include <stdint.h>

uint8_t stream_read(uint8_t* stream);

uint8_t serial_receive(uint8_t* returnBuf, uint8_t size, uint8_t* stream, uint8_t streamSize);

uint8_t test_first_0();
uint8_t test_long_frame();
uint8_t test_many_0();
uint8_t test_single_char();


#endif

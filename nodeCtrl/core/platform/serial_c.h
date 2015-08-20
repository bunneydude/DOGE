#ifndef SERIAL_C_H
#define SERIAL_C_H
#include "platform.h"
#include "../packet.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
  NONE = 0,
  NEWLINE
}printOptions;

#ifdef __LPC8XX__
void print_decimal(uint32_t buf, printOptions options);
void print_hex(uint32_t buf, printOptions options);
#elif defined(MSP430)
void print_decimal(uint16_t buf, printOptions options);
void print_hex(uint16_t buf, printOptions options);
#endif
void print_string(char const *buf, printOptions options);
void print_packet(dogePacket* packet);
void print_bytes(uint8_t* bytes);

#ifdef __cplusplus
}
#endif
#endif

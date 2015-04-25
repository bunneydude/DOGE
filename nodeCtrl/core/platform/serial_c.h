#ifndef SERIAL_C_H
#define SERIAL_C_H
#include "../packet.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
  NONE = 0,
  NEWLINE
}printOptions;

void print_decimal(uint16_t buf, printOptions options);
void print_string(char *buf, printOptions options);
void print_hex(uint16_t buf, printOptions options);
void print_packet(rawPacket* packet);

#ifdef __cplusplus
}
#endif
#endif

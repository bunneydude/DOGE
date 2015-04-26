#include <SPI.h>
#include "serial_c.h"

void print_decimal(uint16_t buf, printOptions options)
{
  if (options == NEWLINE)
  {
    Serial.println(buf);
  }
  else
  {
    Serial.print(buf);
  }
}

void print_string(char *buf, printOptions options)
{
  if (options == NEWLINE)
  {
    Serial.println(buf);
  }
  else
  {
    Serial.print(buf);
  }
}

void print_hex(uint16_t buf, printOptions options)
{
  if (options == NEWLINE)
  {
    Serial.println(buf, HEX);
  }
  else
  {
    Serial.print(buf, HEX);
  }
}

void print_packet(rawPacket* packet){
  uint8_t i = 0;
  uint8_t* bytes = (uint8_t*)(packet);

  Serial.println("Packet");
  Serial.print("   ["); 
  for(i=0; i < (sizeof(packetHdr) + 1 + packet->size); i++){
    Serial.print(bytes[i]);
  }
  Serial.println("]");
}

#include "serial_c.h"
#ifdef MSP430
#include <SPI.h>
#else
#include <stdio.h>
#endif

#if defined(MSP430) && (DBG == 1)
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

void print_string(char const *buf, printOptions options)
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
#else
void print_decimal(uint16_t buf, printOptions options){}
void print_string(char const *buf, printOptions options){}
void print_hex(uint16_t buf, printOptions options){}
#endif

#ifdef MSP430
#if (DBG == 1)
void print_packet(dogePacket* packet){
  uint8_t i = 0;
  uint8_t* bytes = (uint8_t*)(packet);
  
  switch(GET_HEADER_TYPE(packet->hdr.type)){
    case(RAW_PACKET):
      Serial.print("Raw ");
      break;
    case(SIGNALING_BROADCAST_BEACON):
      Serial.print("Signaling Broadcast ");
      break;
    case(SIGNALING_UNICAST_BEACON):
      Serial.print("Signaling Unicast ");
      break;
    case(LINK_LAYER_PACKET):
      Serial.print("Link Layer ");
      break;
  }
  Serial.println("Packet");
  Serial.print("   [");
  for(i=0; i < RAW_PACKET_TOTAL_SIZE(packet); i++){
    Serial.print(bytes[i], HEX);
    Serial.print(" ");
  }
  Serial.println("]");
}
#else
void print_packet(dogePacket* packet){}
#endif
#else
void print_packet(dogePacket* packet){
  uint8_t i = 0;
  uint8_t* bytes = (uint8_t*)(packet);
  switch(GET_HEADER_TYPE(packet->hdr.type)){
    case(RAW_PACKET):
      printf("Raw ");
      break;
    case(SIGNALING_BROADCAST_BEACON):
      printf("Signaling Broadcast ");
      break;
    case(SIGNALING_UNICAST_BEACON):
      printf("Signaling Unicast ");
      break;
    case(LINK_LAYER_PACKET):
      printf("Link Layer ");
      break;
  }
  printf("Packet\n");
  printf("   [");
  for(i=0; i < RAW_PACKET_TOTAL_SIZE(packet); i++){
    printf("%02X ", bytes[i]);
  }
  printf("]\n");
}
#endif

#if defined(MSP430) && (DBG == 1)
void print_bytes(uint8_t* bytes){
  uint8_t i;
  /* Print 2 bytes */
  Serial.print("   ["); 
  for(i=0; i < sizeof(uint16_t); i++){
    Serial.print(bytes[i], HEX);
    Serial.print(" ");
  }
  Serial.println("]");
}
#else
void print_bytes(uint8_t* bytes){}
#endif

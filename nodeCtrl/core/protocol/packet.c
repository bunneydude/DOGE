/**@file Contains definitions for packet functions */
#include "../packet.h"
#include <stddef.h>
#include "../nodeCtrl.h"
#include "../platform/serial_c.h"
#include "type.h"

void copy_bytes(uint8_t* dst, uint8_t* src, uint8_t size)
{
  uint8_t i = 0;
  for (i = 0; i < size; i++){
    ((uint8_t*)dst)[i] = ((uint8_t*)src)[i];
  }
}

void copy_doge_packet(dogePacket* dst, dogePacket* src)
{
  uint8_t i = 0;
  for (i = 0; i < sizeof(dogePacket); i++){
    ((uint8_t*)dst)[i] = ((uint8_t*)src)[i];
  }
}

void copy_raw_packet_data(rawPacket* dst, rawPacket* src)
{
  uint8_t i = 0;
  for (i = 0; i < RAW_PACKET_DATA_SIZE(src); i++){
    dst->data[i] = src->data[i];
  }
}
 
uint8_t packet_payload_end(dogePacket* packet)
{
   uint8_t end = 0;
   if (!HEADER_TYPE_EQUALS(packet->hdr.type, RAW_PACKET) &&
        IS_HEADER_TYPE_ACK(packet->hdr.type)){
      end = PACKET_PAYLOAD_OFFSET + sizeof(((packetAck*)0)->errorCode);
   }
   else if (HEADER_TYPE_EQUALS(packet->hdr.type, RAW_PACKET)){
      end = RAW_PACKET_DATA_CRC_END((rawPacket*)packet);
   }
   else if (HEADER_TYPE_EQUALS(packet->hdr.type, SIGNALING_BROADCAST_BEACON)){
      end = PACKET_PAYLOAD_OFFSET;
   }
   else if (HEADER_TYPE_EQUALS(packet->hdr.type, SIGNALING_UNICAST_BEACON)){
      end = PACKET_PAYLOAD_OFFSET;
   }
   return end;
}

/** Add the packet checksum to the packet header crc */
void add_packet_crc(dogePacket* packet)
{
   int8_t i = 0;
   int16_t sum = 0;
   uint8_t packetEnd = 0;
   uint8_t* bytes = (uint8_t*)packet;
   /* Compute header sum */
   for (i = 0; i < NUM_PACKET_HEADER_CRC_BYTES; i++){
      sum += bytes[i];
   }
   packetEnd = packet_payload_end(packet);
   /* Add header sum to payload sum */
   for (i = PACKET_PAYLOAD_OFFSET; i < packetEnd; i++){
      sum += bytes[i];
   }
   packet->hdr.crc = sum & 0xFF;
}

/** Checks the packet checksum to make sure it matches the packet header crc */
uint8_t check_packet_crc(dogePacket* packet)
{
   int8_t i;
   int16_t sum = 0;
   uint8_t packetEnd = 0;
   uint8_t* bytes = (uint8_t*)packet;
   /* Return an error immediately if a raw packet has an invalid size */
   if ((HEADER_TYPE_EQUALS(packet->hdr.type, RAW_PACKET)) && 
       (RAW_PACKET_DATA_SIZE(packet) > MAX_RAW_PACKET_PAYLOAD_SIZE)){
     return ERR_CHECKSUM;
   }
   /* Compute header sum */
   for (i = 0; i < NUM_PACKET_HEADER_CRC_BYTES; i++){
      sum += bytes[i];
   }
   packetEnd = packet_payload_end(packet);
   /* Add header sum to payload sum */
   for (i = PACKET_PAYLOAD_OFFSET; i < packetEnd; i++){
      sum += bytes[i];
   }
   if (packet->hdr.crc == (sum & 0xFF)){
      return SUCCESS;
   }
   else{
      return ERR_CHECKSUM;
   }
}

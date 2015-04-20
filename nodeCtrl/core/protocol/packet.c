/**@file Contains definitions for packet functions */
#include <packet.h>
#include <stddef.h>
#include <nodeCtrl.h>
#include <type.h>


/** Add the raw packet checksum to the packet header crc */
void add_raw_packet_crc(rawPacket* packet)
{
   int8_t i;
   int16_t sum = 0;
   uint8_t* bytes = (uint8_t*)packet;
   for (i = 0; i < NUM_PACKET_HEADER_CRC_BYTES; i++){
      sum += bytes[i];
   }
   for (i = RAW_PACKET_DATA_CRC_BEGIN; i < RAW_PACKET_DATA_CRC_END(packet); i++){
      sum += bytes[i];
   }
   packet->hdr.crc = sum & 0xFF;
}

/** Checks the raw packet checksum to make sure it matches the packet header crc */
uint8_t check_raw_packet_crc(rawPacket* packet)
{
   int8_t i;
   int16_t sum = 0;
   uint8_t* bytes = (uint8_t*)packet;

   if (packet->size > MAX_RAW_PACKET_PAYLOAD_SIZE)
   {
      return ERR_CHECKSUM;
   }
   for (i = 0; i < NUM_PACKET_HEADER_CRC_BYTES; i++){
      sum += bytes[i];
   }
   for (i = RAW_PACKET_DATA_CRC_BEGIN; i < RAW_PACKET_DATA_CRC_END(packet); i++){
      sum += bytes[i];
   }
   if (packet->hdr.crc == (sum & 0xFF)){
      return SUCCESS;
   }
   else{
      return ERR_CHECKSUM;
   }
}

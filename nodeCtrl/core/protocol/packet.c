/**@file Contains definitions for packet functions */
#include <packet.h>
#include <stddef.h>

#define NUM_PACKET_HEADER_CRC_BYTES (sizeof(packetHdr) - sizeof(((packetHdr*)0)->crc))
#define RAW_PACKET_DATA_OFFSET      (offsetof(rawPacket, data))

/** Add the raw packet checksum to the packet header crc */
void add_raw_packet_crc(rawPacket* packet)
{
	int8_t i;
	int16_t sum = 0;
	uint8_t* bytes = (uint8_t*)packet;
	for (i = 0; i < NUM_PACKET_HEADER_CRC_BYTES; i++){
		sum += bytes[i];
	}
	for (i = RAW_PACKET_DATA_OFFSET; i < (RAW_PACKET_DATA_OFFSET + packet->size); i++){
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
	for (i = 0; i < NUM_PACKET_HEADER_CRC_BYTES; i++){
		sum += bytes[i];
	}
	for (i = RAW_PACKET_DATA_OFFSET; i < (RAW_PACKET_DATA_OFFSET + packet->size); i++){
		sum += bytes[i];
	}
   if (packet->hdr.crc == (sum & 0xFF)){
      return 0;
   }
   else{
      return 1;
   }
}

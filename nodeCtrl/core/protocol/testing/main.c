#include <stdint.h>
#include <malloc.h>
#include <protocol.h>
#include <packet.h>
#include <string.h>
#include <assert.h>

#define TEST_PACKET_ID 0x3
#define TEST_RTA 0xC
#define TEST_TYPE_ACK 0x1
#define TEST_HEADER_TYPE RAW_PACKET

int main()
{
	uint8_t status = 0;
   struct Protocol* obj = (struct Protocol*)malloc(sizeof(struct Protocol*));
   uint8_t* buf = (uint8_t*)malloc(100);
   uint8_t* response = (uint8_t*)malloc(100);

   packetHdr hdr;
   uint8_t packetId;
   uint8_t type;
   uint8_t typeAck;
   uint8_t rta;

   memset(&hdr, 0, sizeof(packetHdr));
   SET_HEADER_TYPE_ACK(hdr.type, TEST_TYPE_ACK);
   SET_HEADER_TYPE(hdr.type, TEST_HEADER_TYPE);
   SET_TXINFO_PACKET_ID(hdr.txInfo, TEST_PACKET_ID);
   SET_TXINFO_RTA(hdr.txInfo, TEST_RTA);
   GET_HEADER_TYPE_ACK(hdr.type, typeAck);
   GET_HEADER_TYPE(hdr.type, type);
   GET_TXINFO_PACKET_ID(hdr.txInfo, packetId);
   GET_TXINFO_RTA(hdr.txInfo, rta);
   
   assert(typeAck == TEST_TYPE_ACK);
   assert(type == TEST_HEADER_TYPE);
   assert(packetId == TEST_PACKET_ID);
   assert(rta == TEST_RTA);

	Protocol_init(obj);
   /*buf[0] = CMD_READ_REG; //CMD*/
   /*buf[1] = 0x2; //size*/
   /*buf[2] = 0x40; //payload*/
   /*buf[3] = 0x40; //payload*/
   /*buf[4] = 0 - (buf[0] + buf[1] + buf[2] + buf[3]);*/
   /*Protocol_parse_packet(obj, buf, response);*/
   return status;
}

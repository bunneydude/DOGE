#include <stdint.h>
#include <malloc.h>
#include <protocol.h>
#include <packet.h>
#include <string.h>
#include <assert.h>

#define TEST_PACKET_ID   0x3
#define TEST_RTA         0xC
#define TEST_TYPE_ACK    0x1
#define TEST_SRC_NODE_ID 0x9
#define TEST_DST_NODE_ID 0x10
#define TEST_TTL         0x5
#define TEST_PACKET_SIZE 16
#define TEST_PACKET_DATA 0xDA
#define TEST_HEADER_TYPE RAW_PACKET

static void test_packet();

int main()
{
   uint8_t status = 0;
   struct Protocol* obj = (struct Protocol*)malloc(sizeof(struct Protocol*));
   uint8_t* buf = (uint8_t*)malloc(100);
   uint8_t* response = (uint8_t*)malloc(100);
   test_packet();
	Protocol_init(obj);
   /*buf[0] = CMD_READ_REG; //CMD*/
   /*buf[1] = 0x2; //size*/
   /*buf[2] = 0x40; //payload*/
   /*buf[3] = 0x40; //payload*/
   /*buf[4] = 0 - (buf[0] + buf[1] + buf[2] + buf[3]);*/
   /*Protocol_parse_packet(obj, buf, response);*/
   return status;
}

void test_packet()
{
   uint8_t status = 0;
   uint8_t packetId;
   uint8_t type;
   uint8_t typeAck;
   uint8_t rta;
   packetHdr hdr;
   rawPacket packet;

   memset(&hdr, 0, sizeof(packetHdr));
	memset(&packet, 0, sizeof(packet));
   SET_HEADER_TYPE_ACK(hdr.type, TEST_TYPE_ACK);
   SET_HEADER_TYPE(hdr.type, TEST_HEADER_TYPE);
   SET_TXINFO_PACKET_ID(hdr.txInfo, TEST_PACKET_ID);
   SET_TXINFO_RTA(hdr.txInfo, TEST_RTA);
	hdr.src = TEST_SRC_NODE_ID;
	hdr.dst = TEST_DST_NODE_ID;
	hdr.ttl = TEST_TTL;
	packet.hdr = hdr;
   memset((void*)((void*)(&packet) + sizeof(packetHdr) + sizeof(packet.size)), TEST_PACKET_DATA, TEST_PACKET_SIZE);
   packet.size = TEST_PACKET_SIZE;

   GET_HEADER_TYPE_ACK(hdr.type, typeAck);
   GET_HEADER_TYPE(hdr.type, type);
   GET_TXINFO_PACKET_ID(hdr.txInfo, packetId);
   GET_TXINFO_RTA(hdr.txInfo, rta);
   add_raw_packet_crc(&packet);
   
   assert(typeAck == TEST_TYPE_ACK);
   assert(type == TEST_HEADER_TYPE);
   assert(packetId == TEST_PACKET_ID);
   assert(rta == TEST_RTA);

   status = check_raw_packet_crc(&packet);
   assert(status == 0);
   
   SET_HEADER_TYPE(packet.hdr.type, BULK_DATA_READ_REQ);
   status = check_raw_packet_crc(&packet);
   assert(status == 1);
}

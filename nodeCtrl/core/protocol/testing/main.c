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


#define TEST_CMD_READ_REG_ADDR 0x2

static void test_raw_packet();
static void test_app_packet();

int main(void) __attribute__((noreturn));
int main()
{
   test_raw_packet();
   test_app_packet();
   while(1);
}

void test_app_packet()
{
   struct Protocol obj;
   rawPacket messageRaw;
   rawPacket responseRaw;
   appPacket* messageApp;
   appPacket* responseApp;
   packetAttr messageAttr;
   packetAttr responseAttr;
   uint8_t status;
 
   Protocol_init(&obj);
   memset(&messageRaw, 0, sizeof(rawPacket));
   memset(&responseRaw, 0, sizeof(rawPacket));
   messageApp = (appPacket*)messageRaw.data;
   responseApp = (appPacket*)responseRaw.data;
   /* Test CMD READ REG packet */
   // Form message packet
   application_form_packet(messageApp, &messageAttr, CMD_READ_REG, TEST_CMD_READ_REG_ADDR, 0x0);
   link_layer_form_packet(&messageRaw, &messageAttr, RAW_PACKET, TEST_SRC_NODE_ID, TEST_DST_NODE_ID);
   assert(messageRaw.size == CMD_READ_REG_DATA_SIZE);
   // Check response
   status = link_layer_parse_packet(&obj, &messageRaw, &responseRaw);
   assert(status == TRANSMIT_RESPONSE);
   assert(responseRaw.hdr.src == TEST_DST_NODE_ID);
   assert(responseRaw.hdr.dst == TEST_SRC_NODE_ID);
   assert(IS_HEADER_TYPE_ACK(responseRaw.hdr.type) && HEADER_TYPE_EQUALS(responseRaw.hdr.type, RAW_PACKET));
   assert(responseRaw.size == CMD_ACK_DATA_SIZE);
   assert(responseApp->cmd == CMD_ACK);
   assert(check_raw_packet_crc(&responseRaw) == 0);
}

void test_raw_packet()
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

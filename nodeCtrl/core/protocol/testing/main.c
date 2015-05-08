#include <stdint.h>
#include <malloc.h>
#include <protocol.h>
#include <packet.h>
#include <string.h>
#include <assert.h>
#include "../network/network.h"
#include "../platform/platform.h"
#ifdef LINUX
#include <stdio.h>
#endif

#define TEST_PACKET_ID      0x3
#define TEST_RTA            0xC
#define TEST_TYPE_ACK       0x1
#define TEST_TYPE_NO_ACK    0x0
#define TEST_SRC_NODE_ID    0x6
#define TEST_DST_NODE_ID    0x8
#define TEST_SH_SRC_NODE_ID 0x7
#define TEST_SH_DST_NODE_ID 0x8
#define TEST_SH_LQE         0x29
#define TEST_TTL            0x5
#define TEST_PACKET_SIZE    16
#define TEST_PACKET_DATA    0xDA
#define TEST_HEADER_TYPE RAW_PACKET

#define RADIO_ID_915 (0x1)
#define NETWORK_ID_0 (0x0)

#define TEST_CMD_READ_REG_ADDR 0x2

static void test_raw_packet();
static void test_app_packet();
static void test_signaling_packets();
static void test_read_write_mem_packets();

int main(void) __attribute__((noreturn));
int main()
{
   test_raw_packet();
   test_app_packet();
   test_signaling_packets();
   test_read_write_mem_packets();
   while(1);
}

void test_app_packet()
{
   struct Protocol obj;
   dogePacket message;
   dogePacket response;
   appPacket* messageApp;
   appPacket* responseApp;
   packetAttr messageAttr;
   packetAttr responseAttr;
   uint8_t status;
   uint8_t bytes[MAX_RAW_PACKET_PAYLOAD_SIZE];

   Protocol_init(&obj);
   memset(&message, 0, sizeof(dogePacket));
   memset(&response, 0, sizeof(dogePacket));
   messageApp  = (appPacket*)((void*)&message + RAW_PACKET_DATA_OFFSET);
   responseApp = (appPacket*)((void*)&response + RAW_PACKET_DATA_OFFSET);
   /* Test CMD READ REG packet */
   // Form message packet
   // If the application layer is processing this packet, then the Single Hop
   // Destination Node ID must equal the Destination Node ID
   assert(TEST_SH_DST_NODE_ID == TEST_DST_NODE_ID);
   application_form_packet(messageApp, &messageAttr, CMD_READ_REG, TEST_CMD_READ_REG_ADDR, 0x0);
   memcpy(bytes, (void *)&message + RAW_PACKET_DATA_OFFSET, MAX_RAW_PACKET_PAYLOAD_SIZE);
   assert(bytes[0] == CMD_READ_REG);
   assert(bytes[1] == TEST_CMD_READ_REG_ADDR);
   link_layer_form_packet(&message, &messageAttr, RAW_PACKET, TEST_SRC_NODE_ID, TEST_DST_NODE_ID, TEST_SH_SRC_NODE_ID, TEST_SH_DST_NODE_ID);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&message);
   // Check to make sure application data is intact
   assert(memcmp(bytes, (void *)&message + RAW_PACKET_DATA_OFFSET, MAX_RAW_PACKET_PAYLOAD_SIZE) == 0);
   assert(RAW_PACKET_DATA_SIZE(&message) == CMD_READ_REG_DATA_SIZE);
   // Check response
   status = link_layer_parse_packet(&obj, &message, &response);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&response);
   assert(status == TRANSMIT_RESPONSE);
   assert(response.hdr.src == TEST_DST_NODE_ID);
   assert(response.hdr.dst == TEST_SRC_NODE_ID);
   assert(response.hdr.shSrc == TEST_SH_DST_NODE_ID);
   assert(response.hdr.shDst == TEST_SH_SRC_NODE_ID);
   assert(IS_HEADER_TYPE_ACK(response.hdr.type) && HEADER_TYPE_EQUALS(response.hdr.type, RAW_PACKET));
   assert(RAW_PACKET_DATA_SIZE(&response) == CMD_ACK_DATA_SIZE);
   assert(responseApp->cmd == CMD_ACK);
   assert(check_packet_crc(&response) == 0);
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
   hdr.shSrc = TEST_SH_SRC_NODE_ID;
   hdr.shDst = TEST_SH_DST_NODE_ID;
   hdr.ttl = TEST_TTL;
   packet.hdr = hdr;
   memset((void*)((void*)(&packet) + sizeof(packetHdr) + sizeof(packet.size)), TEST_PACKET_DATA, TEST_PACKET_SIZE);
   packet.size = TEST_PACKET_SIZE;

   typeAck  = GET_HEADER_TYPE_ACK(hdr.type);
   type     = GET_HEADER_TYPE(hdr.type);
   packetId = GET_TXINFO_PACKET_ID(hdr.txInfo);
   rta      = GET_TXINFO_RTA(hdr.txInfo);
   add_packet_crc((dogePacket*)&packet);

   assert(typeAck == TEST_TYPE_ACK);
   assert(type == TEST_HEADER_TYPE);
   assert(packetId == TEST_PACKET_ID);
   assert(rta == TEST_RTA);

   status = check_packet_crc((dogePacket*)&packet);
   assert(status == 0);
   
   SET_HEADER_TYPE(packet.hdr.type, BULK_DATA_READ_REQ);
   status = check_packet_crc((dogePacket*)&packet);
   assert(status == 1);
}

void test_signaling_packets()
{
   uint8_t status = 0;
   uint8_t packetId;
   uint8_t rta;
   struct Protocol obj;
   dogePacket message;
   dogePacket response;
   appPacket* messageApp;
   appPacket* responseApp;
   packetAttr messageAttr;
   packetAttr responseAttr;

   memset(&message, 0, sizeof(dogePacket));
   memset(&response, 0, sizeof(dogePacket));
   link_layer_form_packet(&message, &messageAttr, SIGNALING_BROADCAST_BEACON, TEST_SRC_NODE_ID, TEST_DST_NODE_ID, TEST_SH_SRC_NODE_ID, TEST_SH_DST_NODE_ID);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&message);
   status = link_layer_parse_packet(&obj, &message, &response);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&response);
   assert(status == TRANSMIT_RESPONSE);
   assert(response.hdr.src == TEST_DST_NODE_ID);
   assert(response.hdr.dst == TEST_SRC_NODE_ID);
   assert(response.hdr.shSrc == TEST_SH_DST_NODE_ID);
   assert(response.hdr.shDst == TEST_SH_SRC_NODE_ID);
   assert(GET_HEADER_TYPE_ACK(response.hdr.type) == TEST_TYPE_NO_ACK);
   assert(GET_HEADER_TYPE(response.hdr.type) == SIGNALING_UNICAST_BEACON);
   status = check_packet_crc((dogePacket*)&response);
   assert(status == 0);

   memset(&message, 0, sizeof(dogePacket));
   memset(&response, 0, sizeof(dogePacket));
   link_layer_form_packet(&message, &messageAttr, SIGNALING_UNICAST_BEACON, TEST_SRC_NODE_ID, TEST_DST_NODE_ID, TEST_SH_SRC_NODE_ID, TEST_SH_DST_NODE_ID);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&message);
   status = link_layer_parse_packet(&obj, &message, &response);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&response);
   assert(status == TRANSMIT_RESPONSE);
   assert(response.hdr.src == TEST_DST_NODE_ID);
   assert(response.hdr.dst == TEST_SRC_NODE_ID);
   assert(response.hdr.shSrc == TEST_SH_DST_NODE_ID);
   assert(response.hdr.shDst == TEST_SH_SRC_NODE_ID);
   assert(GET_HEADER_TYPE_ACK(response.hdr.type) == TEST_TYPE_ACK);
   assert(GET_HEADER_TYPE(response.hdr.type) == SIGNALING_UNICAST_BEACON);
   status = check_packet_crc((dogePacket*)&response);
   assert(status == 0);
}

void test_read_write_mem_packets()
{
   struct Protocol obj;
   dogePacket message;
   dogePacket response;
   appPacket* messageApp;
   appPacket* responseApp;
   packetAttr messageAttr;
   packetAttr responseAttr;
   uint8_t status;
   uint8_t newBytes[MAX_CMD_WRITE_MEM_DATA_SIZE] = {0x06,0x00,0x34,0x10,0x07,0x00,0x21,0x10,0x05,0x00,0x28,0x10};
   uint8_t bytes[MAX_RAW_PACKET_PAYLOAD_SIZE];
   uint8_t i;

   Protocol_init(&obj);
   network_init(NETWORK_DIVISION_DEFAULT);
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
     NODE_ID_2, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0  };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   memset(&message, 0, sizeof(dogePacket));
   memset(&response, 0, sizeof(dogePacket));
   messageApp  = (appPacket*)((void*)&message + RAW_PACKET_DATA_OFFSET);
   responseApp = (appPacket*)((void*)&response + RAW_PACKET_DATA_OFFSET);
   /* Test CMD READ MEM packet */
   // Form message packet
   // If the application layer is processing this packet, then the Single Hop
   // Destination Node ID must equal the Destination Node ID
   assert(TEST_SH_DST_NODE_ID == TEST_DST_NODE_ID);
   application_form_packet(messageApp, &messageAttr, CMD_READ_MEM, MM_NETWORK_BASE, MAX_CMD_READ_MEM_DATA_SIZE);
   //Check app packet is inserted correctly into dogePacket by inspecting bytes
   memcpy(bytes, (void *)&message + RAW_PACKET_DATA_OFFSET, MAX_RAW_PACKET_PAYLOAD_SIZE);
   assert(bytes[0] == CMD_READ_MEM);
   assert(bytes[1] == MM_NETWORK_BASE);
   link_layer_form_packet(&message, &messageAttr, RAW_PACKET, TEST_SRC_NODE_ID, TEST_DST_NODE_ID, TEST_SH_SRC_NODE_ID, TEST_SH_DST_NODE_ID);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&message);
   // Check to make sure application data is intact
   assert(memcmp(bytes, (void *)&message + RAW_PACKET_DATA_OFFSET, MAX_RAW_PACKET_PAYLOAD_SIZE) == 0);
   assert(RAW_PACKET_DATA_SIZE(&message) == CMD_READ_MEM_DATA_SIZE);
   // Check response
   status = link_layer_parse_packet(&obj, &message, &response);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&response);
   assert(status == TRANSMIT_RESPONSE);
   assert(response.hdr.src == TEST_DST_NODE_ID);
   assert(response.hdr.dst == TEST_SRC_NODE_ID);
   assert(response.hdr.shSrc == TEST_SH_DST_NODE_ID);
   assert(response.hdr.shDst == TEST_SH_SRC_NODE_ID);
   assert(IS_HEADER_TYPE_ACK(response.hdr.type) && HEADER_TYPE_EQUALS(response.hdr.type, RAW_PACKET));
   assert(RAW_PACKET_DATA_SIZE(&response) == CMD_READ_MEM_ACK_DATA_SIZE(MAX_CMD_READ_MEM_DATA_SIZE));
   assert(responseApp->cmd == CMD_READ_MEM_ACK);
   assert(check_packet_crc(&response) == 0);
   print_packet(&response);
   /* Test CMD WRITE MEM packet */
   // Form message packet
   memset(&message, 0, sizeof(dogePacket));
   memset(&response, 0, sizeof(dogePacket));
   application_form_packet(messageApp, &messageAttr, CMD_WRITE_MEM, MM_NETWORK_BASE, MAX_CMD_WRITE_MEM_DATA_SIZE);
   copy_bytes(CMD_WRITE_MEM_DATA_ADDRESS(((rawPacket*)&message)->data), newBytes, MAX_CMD_WRITE_MEM_DATA_SIZE);
   link_layer_form_packet(&message, &messageAttr, RAW_PACKET, TEST_SRC_NODE_ID, TEST_DST_NODE_ID, TEST_SH_SRC_NODE_ID, TEST_SH_DST_NODE_ID);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&message);
   // Check response
   status = link_layer_parse_packet(&obj, &message, &response);
   assert(status == TRANSMIT_RESPONSE);
   assert(responseApp->cmd == CMD_WRITE_MEM_ACK);
   assert(responseApp->addr == MM_NETWORK_BASE);
   assert(responseApp->data == MAX_CMD_WRITE_MEM_DATA_SIZE);
   print_packet(&response);
   for (i = MM_NETWORK_BASE; i < MM_NETWORK_BASE + MAX_CMD_WRITE_MEM_DATA_SIZE; i++)
   {
     printf("%02X ", obj.dataRegisters[i]);
   }
   printf("\n");
}

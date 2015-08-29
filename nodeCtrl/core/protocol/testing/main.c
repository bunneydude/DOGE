#include <stdint.h>
#include <malloc.h>
#include <protocol.h>
#include <packet.h>
#include <string.h>
#include <assert.h>
#include "../network/network.h"
#include <platform.h>
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

#define TEST_CMD_READ_REG_ADDR 0x2

static void test_raw_packet();
static void test_app_packet();
static void test_signaling_packets();
static void test_read_write_mem_packets();
static void test_user_app_packets();

dogePacket txPacket;
extern appPacket* const txAppPacket;
appPacket* const txAppPacket = (appPacket*)(&((rawPacket*)(&txPacket))->data);
packetAttr txAttr;

dogePacket rxPacket;
extern appPacket* const rxAppPacket;
appPacket* const rxAppPacket = (appPacket*)(&((rawPacket*)(&rxPacket))->data);
packetAttr rxAttr;

int main(void) __attribute__((noreturn));
int main()
{
   test_raw_packet();
   test_app_packet();
   test_signaling_packets();
   test_read_write_mem_packets();
   test_user_app_packets();
   printf("FINISH\n");
   while(1);
}

void test_app_packet()
{
   struct Protocol obj;
   uint8_t status;
   uint8_t bytes[MAX_RAW_PACKET_PAYLOAD_SIZE];

   Protocol_init(&obj);
   memset(&txPacket, 0, sizeof(dogePacket));
   memset(&rxPacket, 0, sizeof(dogePacket));
   /* Test CMD READ REG packet */
   // Form message packet
   // If the application layer is processing this packet, then the Single Hop
   // Destination Node ID must equal the Destination Node ID
   assert(TEST_SH_DST_NODE_ID == TEST_DST_NODE_ID);
   application_form_packet(txAppPacket, &txAttr, CMD_READ_REG, TEST_CMD_READ_REG_ADDR, 0x0, NULL);
   memcpy(bytes, ((rawPacket*)&txPacket)->data, MAX_RAW_PACKET_PAYLOAD_SIZE);
   assert(bytes[0] == CMD_READ_REG);
   assert(bytes[1] == TEST_CMD_READ_REG_ADDR);
   link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, TEST_SRC_NODE_ID, TEST_DST_NODE_ID, TEST_SH_SRC_NODE_ID, TEST_SH_DST_NODE_ID);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&txPacket);
   // Check to make sure application data is intact
   assert(memcmp(bytes, ((rawPacket*)&txPacket)->data, MAX_RAW_PACKET_PAYLOAD_SIZE) == 0);
   assert(RAW_PACKET_DATA_SIZE(&txPacket) == CMD_READ_REG_DATA_SIZE);

   // Check response
   status = link_layer_parse_packet(&obj, &txPacket, &rxPacket);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&rxPacket);
   assert(status == TRANSMIT_RESPONSE);
   assert(rxPacket.hdr.src == TEST_DST_NODE_ID);
   assert(rxPacket.hdr.dst == TEST_SRC_NODE_ID);
   assert(rxPacket.hdr.shSrc == TEST_SH_DST_NODE_ID);
   assert(rxPacket.hdr.shDst == TEST_SH_SRC_NODE_ID);
   assert(IS_HEADER_TYPE_ACK(rxPacket.hdr.type) && HEADER_TYPE_EQUALS(rxPacket.hdr.type, RAW_PACKET));
   assert(RAW_PACKET_DATA_SIZE(&rxPacket) == CMD_ACK_DATA_SIZE);
   assert(rxAppPacket->cmd == CMD_ACK);
   assert(check_packet_crc(&rxPacket) == 0);
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

   memset(&txPacket, 0, sizeof(dogePacket));
   memset(&rxPacket, 0, sizeof(dogePacket));
   link_layer_form_packet(&txPacket, &txAttr, SIGNALING_BROADCAST_BEACON, TEST_SRC_NODE_ID, TEST_DST_NODE_ID, TEST_SH_SRC_NODE_ID, TEST_SH_DST_NODE_ID);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&txPacket);
   status = link_layer_parse_packet(&obj, &txPacket, &rxPacket);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&rxPacket);
   assert(status == TRANSMIT_RESPONSE);
   assert(rxPacket.hdr.src == TEST_DST_NODE_ID);
   assert(rxPacket.hdr.dst == TEST_SRC_NODE_ID);
   assert(rxPacket.hdr.shSrc == TEST_SH_DST_NODE_ID);
   assert(rxPacket.hdr.shDst == TEST_SH_SRC_NODE_ID);
   assert(GET_HEADER_TYPE_ACK(rxPacket.hdr.type) == TEST_TYPE_NO_ACK);
   assert(GET_HEADER_TYPE(rxPacket.hdr.type) == SIGNALING_UNICAST_BEACON);
   status = check_packet_crc((dogePacket*)&rxPacket);
   assert(status == 0);

   memset(&txPacket, 0, sizeof(dogePacket));
   memset(&rxPacket, 0, sizeof(dogePacket));
   link_layer_form_packet(&txPacket, &txAttr, SIGNALING_UNICAST_BEACON, TEST_SRC_NODE_ID, TEST_DST_NODE_ID, TEST_SH_SRC_NODE_ID, TEST_SH_DST_NODE_ID);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&txPacket);
   status = link_layer_parse_packet(&obj, &txPacket, &rxPacket);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&rxPacket);
   assert(status == TRANSMIT_RESPONSE);
   assert(rxPacket.hdr.src == TEST_DST_NODE_ID);
   assert(rxPacket.hdr.dst == TEST_SRC_NODE_ID);
   assert(rxPacket.hdr.shSrc == TEST_SH_DST_NODE_ID);
   assert(rxPacket.hdr.shDst == TEST_SH_SRC_NODE_ID);
   assert(GET_HEADER_TYPE_ACK(rxPacket.hdr.type) == TEST_TYPE_ACK);
   assert(GET_HEADER_TYPE(rxPacket.hdr.type) == SIGNALING_UNICAST_BEACON);
   status = check_packet_crc((dogePacket*)&rxPacket);
   assert(status == 0);
}

void test_read_write_mem_packets()
{
   struct Protocol obj;
   uint8_t status;
   uint8_t newBytes[MAX_CMD_WRITE_MEM_DATA_SIZE] = {0x06,0x00,0x34,0x10,0x07,0x00,0x21,0x10,0x05,0x00,0x28,0x10};
   uint8_t bytes[MAX_RAW_PACKET_PAYLOAD_SIZE];
   uint8_t i;

   Protocol_init(&obj);
   network_init(NETWORK_DIVISION_DEFAULT);
   //Neighbor Table
   insert_neighbor(NODE_ID_2, TEST_SH_LQE, RADIO_ID_915);
   memset(&txPacket, 0, sizeof(dogePacket));
   memset(&rxPacket, 0, sizeof(dogePacket));
   
   /* Test CMD READ MEM packet */
   // Form message packet
   // If the application layer is processing this packet, then the Single Hop
   // Destination Node ID must equal the Destination Node ID
   assert(TEST_SH_DST_NODE_ID == TEST_DST_NODE_ID);
   application_form_packet(txAppPacket, &txAttr, CMD_READ_MEM, MM_NETWORK_BASE, MAX_CMD_READ_MEM_DATA_SIZE, NULL);
   //Check app packet is inserted correctly into dogePacket by inspecting bytes
   memcpy(bytes, ((rawPacket*)(&txPacket))->data, MAX_RAW_PACKET_PAYLOAD_SIZE);
   assert(bytes[0] == CMD_READ_MEM);
   assert(bytes[1] == MM_NETWORK_BASE);
   link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, TEST_SRC_NODE_ID, TEST_DST_NODE_ID, TEST_SH_SRC_NODE_ID, TEST_SH_DST_NODE_ID);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&txPacket);
   // Check to make sure application data is intact
   assert(memcmp(bytes, ((rawPacket*)(&txPacket))->data, MAX_RAW_PACKET_PAYLOAD_SIZE) == 0);
   assert(RAW_PACKET_DATA_SIZE(&txPacket) == CMD_READ_MEM_DATA_SIZE);

   // Check response
   status = link_layer_parse_packet(&obj, &txPacket, &rxPacket);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&rxPacket);
   assert(status == TRANSMIT_RESPONSE);
   assert(rxPacket.hdr.src == TEST_DST_NODE_ID);
   assert(rxPacket.hdr.dst == TEST_SRC_NODE_ID);
   assert(rxPacket.hdr.shSrc == TEST_SH_DST_NODE_ID);
   assert(rxPacket.hdr.shDst == TEST_SH_SRC_NODE_ID);
   assert(IS_HEADER_TYPE_ACK(rxPacket.hdr.type) && HEADER_TYPE_EQUALS(rxPacket.hdr.type, RAW_PACKET));
   assert(RAW_PACKET_DATA_SIZE(&rxPacket) == CMD_READ_MEM_ACK_DATA_SIZE(MAX_CMD_READ_MEM_DATA_SIZE));
   assert(rxAppPacket->cmd == CMD_READ_MEM_ACK);
   assert(check_packet_crc(&rxPacket) == 0);
   /*print_packet(&rxPacket);*/

   /* Test CMD WRITE MEM packet */
   // Form message packet
   memset(&txPacket, 0, sizeof(dogePacket));
   memset(&rxPacket, 0, sizeof(dogePacket));
   application_form_packet(txAppPacket, &txAttr, CMD_WRITE_MEM, MM_NETWORK_BASE, MAX_CMD_WRITE_MEM_DATA_SIZE, newBytes);
   link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, TEST_SRC_NODE_ID, TEST_DST_NODE_ID, TEST_SH_SRC_NODE_ID, TEST_SH_DST_NODE_ID);
   //Mock reliable transmit/receive crc calculation
   add_packet_crc(&txPacket);
   // Check response
   status = link_layer_parse_packet(&obj, &txPacket, &rxPacket);
   assert(status == TRANSMIT_RESPONSE);
   assert(rxAppPacket->cmd == CMD_WRITE_MEM_ACK);
   assert(rxAppPacket->addr == MM_NETWORK_BASE);
   assert(rxAppPacket->data == MAX_CMD_WRITE_MEM_DATA_SIZE);
   /*print_packet(&rxPacket);*/
   /*for (i = MM_NETWORK_BASE; i < MM_NETWORK_BASE + MAX_CMD_WRITE_MEM_DATA_SIZE; i++)
   {
     printf("%02X ", obj.dataRegisters[i]);
   }
   printf("\n");*/
}

#define USER_PACKET_NUM 4
void test_user_app_packets()
{
   struct Protocol obj;
   dogeStatus status;
   int i;
   uint8_t payload_array[USER_PACKET_NUM][5] = { {0x01, 0x02, 0x03, 0x04, 0x05}, {0x11, 0x12, 0x13, 0x14, 0x15},
                                                 {0x21, 0x22, 0x23, 0x24, 0x25}, {0x01, 0x01, 0x01, 0x01, 0x01} };
   Protocol_init(&obj);
   for (i = 0; i < USER_PACKET_NUM; i++){
      memset(&txPacket, 0, sizeof(dogePacket));
      memset(&rxPacket, 0, sizeof(dogePacket));
      user_application_form_packet((userAppPacket*)txAppPacket, &txAttr, CMD_USER_APP, USER_APP_PAYLOAD_SIZE, &payload_array[i][0]);
      link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, TEST_SRC_NODE_ID, TEST_DST_NODE_ID, TEST_SH_SRC_NODE_ID, TEST_SH_DST_NODE_ID);
      //Mock reliable transmit/receive crc calculation
      add_packet_crc(&txPacket);
      // Check rxPacket
      status = link_layer_parse_packet(&obj, &txPacket, &rxPacket);
      if (*(uint32_t*)(&payload_array[i][0]) != NO_RESPONSE){
         assert(status == TRANSMIT_RESPONSE);
         assert(rxPacket.payload[0] == CMD_USER_APP_ACK_DATA_SIZE);
         assert(rxPacket.payload[1] == CMD_USER_APP_ACK);
      }else{
         assert(status == NO_TRANSMIT);
      }
   }
}

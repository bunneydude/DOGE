#include "static_routes.h"

extern dogePacket txPacket;
extern appPacket* const txAppPacket;
extern packetAttr txAttr;

extern dogePacket rxPacket;
extern appPacket* const rxAppPacket;
extern packetAttr rxAttr;

void insert_neighbor(uint16_t nodeID, uint16_t LQE)
{
   //Insert neighbor into Neighbor Table
   struct neighborEntry neighborEntry = {
      nodeID, LQE, RADIO_ID_915, NETWORK_ID_0 };
   network_insert((union networkEntry*)&neighborEntry, NEIGHBOR_ENTRY);
}

void insert_route(uint16_t dstNodeID, uint16_t neighborID, uint16_t LQE)
{
   uint8_t index;
   if (network_has_neighbor(neighborID, &index, RADIO_ID_915, FALSE)){
      struct routingEntry routingEntry = {
         dstNodeID, LQE, index };
      network_insert((union networkEntry*)&routingEntry, ROUTING_ENTRY);
   }
}

void setup_iotg_demo_grid()
{
#if (MY_NODE_ID == ROOT_NODE)
   //ROOT NODE CONFIG IS IN PYTHON
#elif (MY_NODE_ID == NODE_ID_2)
   //Neighbor Table
   insert_neighbor(ROOT_NODE, MAX_LQE);
   insert_neighbor(NODE_ID_3, MAX_LQE);
   insert_neighbor(NODE_ID_5, MAX_LQE);
   insert_neighbor(NODE_ID_6, MAX_LQE);
   //Routing Table
   insert_route(NODE_ID_10, NODE_ID_3, MAX_LQE);
   insert_route(NODE_ID_10, NODE_ID_5, MAX_LQE);
   insert_route(NODE_ID_10, NODE_ID_6, MAX_LQE);
#elif (MY_NODE_ID == NODE_ID_3)
   //Neighbor Table
   insert_neighbor(ROOT_NODE, MAX_LQE);
   insert_neighbor(NODE_ID_2, MAX_LQE);
   insert_neighbor(NODE_ID_4, MAX_LQE);
   //Routing Table
   insert_route(ROOT_NODE, NODE_ID_2, MAX_LQE);
   insert_route(NODE_ID_10, NODE_ID_4, MAX_LQE);
#elif (MY_NODE_ID == NODE_ID_4)
   //Neighbor Table
   insert_neighbor(ROOT_NODE, MAX_LQE);
   insert_neighbor(NODE_ID_3, MAX_LQE);
   insert_neighbor(NODE_ID_7, MAX_LQE);
   //Routing Table
   insert_route(ROOT_NODE, NODE_ID_3, MAX_LQE);
   insert_route(NODE_ID_10, NODE_ID_7, MAX_LQE);
#elif (MY_NODE_ID == NODE_ID_5)
   //Neighbor Table
   insert_neighbor(ROOT_NODE, MAX_LQE);
   insert_neighbor(NODE_ID_2, MAX_LQE);
   insert_neighbor(NODE_ID_8, MAX_LQE);
   //Routing Table
   insert_route(ROOT_NODE, NODE_ID_2, MAX_LQE);
   insert_route(NODE_ID_10, NODE_ID_8, MAX_LQE);
#elif (MY_NODE_ID == NODE_ID_6)
   //Neighbor Table
   insert_neighbor(ROOT_NODE, MAX_LQE);
   insert_neighbor(NODE_ID_2, MAX_LQE);
   insert_neighbor(NODE_ID_7, MAX_LQE);
   insert_neighbor(NODE_ID_9, MAX_LQE);
   //Routing Table
   insert_route(ROOT_NODE, NODE_ID_2, MAX_LQE);
   insert_route(NODE_ID_10, NODE_ID_7, MAX_LQE);
   insert_route(NODE_ID_10, NODE_ID_9, MAX_LQE);
#elif (MY_NODE_ID == NODE_ID_7)
   //Neighbor Table
   insert_neighbor(ROOT_NODE, MAX_LQE);
   insert_neighbor(NODE_ID_4, MAX_LQE);
   insert_neighbor(NODE_ID_6, MAX_LQE);
   insert_neighbor(NODE_ID_10, MAX_LQE);
   //Routing Table
   insert_route(ROOT_NODE, NODE_ID_4, MAX_LQE);
   insert_route(NODE_ID_10, NODE_ID_6, MAX_LQE);
#elif (MY_NODE_ID == NODE_ID_8)
   //Neighbor Table
   insert_neighbor(ROOT_NODE, MAX_LQE);
   insert_neighbor(NODE_ID_5, MAX_LQE);
   insert_neighbor(NODE_ID_9, MAX_LQE);
   //Routing Table
   insert_route(ROOT_NODE, NODE_ID_5, MAX_LQE);
   insert_route(NODE_ID_10, NODE_ID_9, MAX_LQE);
#elif (MY_NODE_ID == NODE_ID_9)
   //Neighbor Table
   insert_neighbor(ROOT_NODE, MAX_LQE);
   insert_neighbor(NODE_ID_6, MAX_LQE);
   insert_neighbor(NODE_ID_8, MAX_LQE);
   insert_neighbor(NODE_ID_10, MAX_LQE);
   //Routing Table
   insert_route(NODE_ID_10, NODE_ID_6, MAX_LQE);
   insert_route(ROOT_NODE, NODE_ID_8, MAX_LQE);
#elif (MY_NODE_ID == NODE_ID_10)
   //Neighbor Table
   insert_neighbor(ROOT_NODE, MAX_LQE);
   insert_neighbor(NODE_ID_7, MAX_LQE);
   insert_neighbor(NODE_ID_9, MAX_LQE);
   //Routing Table
   insert_route(ROOT_NODE, NODE_ID_7, MAX_LQE);
   insert_route(ROOT_NODE, NODE_ID_9, MAX_LQE);
#endif
}

uint8_t static_route_mm_handler(uint8_t rw, uint8_t addr, uint8_t* data, uint8_t mask)
{
   /*
   switch(addr)
   {
      case(NRF24_NODE_11_OFFSET):
         print_string("SENDING TO NODE 11", NEWLINE);
         break;
      case(NRF24_NODE_12_OFFSET):
         print_string("SENDING TO NODE 12", NEWLINE);
         break;
      case(NRF24_NODE_13_OFFSET):
         print_string("SENDING TO NODE 13", NEWLINE);
         break;
      case(NRF24_NODE_14_OFFSET):
         print_string("SENDING TO NODE 14", NEWLINE);
         break;
      case(NRF24_NODE_15_OFFSET):
         print_string("SENDING TO NODE 15", NEWLINE);
         break;
      case(NRF24_NODE_16_OFFSET):
         print_string("SENDING TO NODE 16", NEWLINE);
         break;
      case(NRF24_NODE_17_OFFSET):
         print_string("SENDING TO NODE 17", NEWLINE);
         break;
      case(NRF24_NODE_18_OFFSET):
         print_string("SENDING TO NODE 18", NEWLINE);
         break;
   }
   */
   application_form_packet(txAppPacket, &txAttr, CMD_WRITE_REG, NRF24_DEFAULT_APP_ADDRESS, *data, NULL);
   link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, MY_NODE_ID, NRF24_NODE_OFFSET_TO_ID(addr), MY_NODE_ID, NRF24_NODE_OFFSET_TO_ID(addr));
   add_packet_crc(&txPacket);
   //print_packet(&txPacket);
#ifdef DUAL_RADIO
   SPI.begin();
   nrf24_send(0, (uint8_t*)(&txPacket), MAX_DATA_LENGTH);
   while(nrf24_isSending());
#endif
   return 0;
}

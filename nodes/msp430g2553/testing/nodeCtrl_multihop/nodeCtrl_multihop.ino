#include <stdint.h>
#include <memory_map.h>
#include <network.h>
#include <protocol.h>
#include <neighbor-config.h>
#include <neighbor.h>
#include <gpio.h>
#include <msp430_gpio.h>
#include <adc.h>
#include <SPI.h>
#include <AIR430BoostFCC.h>
#include <dsp.h>
#include <platform.h>
#include <nrfLegacy.h>

/*******************************************************************************
 * Test 1
 * Node 1 uses hard coded neighbor tables/routing tables to send a packet to the
 * furthest node (Node 3).
 *******************************************************************************/

/*******************************************************************************
 * Test 2
 * Node 1 uses hard coded neighbor tables/routing tables to send a packet to the
 * furthest node (Node 4).
 *******************************************************************************/

/*******************************************************************************
 * Test 3
 * demo_grid.py
 *******************************************************************************/

#define TEST_ID 0x2
#define TEST_SH_LQE 0x1
#define TEST_MH_LQE 0x1

// Data to write to radio TX FIFO (60 bytes MAX.)
dogePacket txPacket;
appPacket* const txAppPacket = (appPacket*)(&((rawPacket*)(&txPacket))->data);
packetAttr txAttr;

// Data to read from radio RX FIFO (60 bytes MAX.)
dogePacket rxPacket;
appPacket* const rxAppPacket = (appPacket*)(&((rawPacket*)(&rxPacket))->data);
packetAttr rxAttr;

struct Protocol spiProtocol;
uint8_t sendResponse = 0;
uint8_t hbt_output = 0x1;

uint8_t returnData = 0;
uint8_t i = 0;
uint8_t rawADC = 0;
uint8_t tempIndex;
union networkEntry tempEntry;

void setup()
{
   print_string("START",NEWLINE);
   setup_timer_hw();
   Protocol_init(&spiProtocol);
   network_init(NETWORK_DIVISION_DEFAULT);
   dsp_init(5,0);
   memoryMapRegionMethods.gpio_handler = gpio_mm_handler;
   memoryMapRegionMethods.adc_handler = adc_mm_handler;
   memoryMapRegionMethods.static_route_handler = static_route_mm_handler;

   Serial.begin(9600);

   // The radio library uses the SPI library internally, this call initializes
   // SPI/CSn and GDO0 lines. Also setup initial address, channel, and TX power.
   Radio.begin(ADDRESS_BROADCAST, CHANNEL_1, POWER_MAX); 

   pinMode(RED_LED, OUTPUT);
   digitalWrite(RED_LED, hbt_output);   // set the LED on

#if (TEST_ID == 0x1)
   uint8_t index;
#if (MY_NODE_ID == ROOT_NODE)
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      NODE_ID_2, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_2, &index, RADIO_ID_915, FALSE); 
   struct routingEntry routingEntry1 = {
      NODE_ID_3, TEST_MH_LQE, index };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
#elif (MY_NODE_ID == NODE_ID_2)
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      ROOT_NODE, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   struct neighborEntry neighborEntry2 = {
      NODE_ID_3, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   network_insert((union networkEntry*)&neighborEntry2, NEIGHBOR_ENTRY);
#elif (MY_NODE_ID == NODE_ID_3)
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      NODE_ID_2, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_2, &index, RADIO_ID_915, FALSE);
   struct routingEntry routingEntry1 = {
      ROOT_NODE, TEST_MH_LQE, index };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
#endif
#elif (TEST_ID == 0x2)
   uint8_t index;
#if (MY_NODE_ID == ROOT_NODE)
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      NODE_ID_2, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_2, &index, RADIO_ID_915, FALSE);
   struct routingEntry routingEntry1 = {
      NODE_ID_3, TEST_MH_LQE, index };
   struct routingEntry routingEntry2 = {
      NODE_ID_4, TEST_MH_LQE, index };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
   network_insert((union networkEntry*)&routingEntry2, ROUTING_ENTRY);
#elif (MY_NODE_ID == NODE_ID_2)
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      ROOT_NODE, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   struct neighborEntry neighborEntry2 = {
      NODE_ID_3, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   network_insert((union networkEntry*)&neighborEntry2, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_3, &index, RADIO_ID_915, FALSE);
   struct routingEntry routingEntry1 = {
      NODE_ID_4, TEST_MH_LQE, index };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
#elif (MY_NODE_ID == NODE_ID_3)
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      NODE_ID_2, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   struct neighborEntry neighborEntry2 = {
      NODE_ID_4, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   network_insert((union networkEntry*)&neighborEntry2, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_2, &index, RADIO_ID_915, FALSE);
   struct routingEntry routingEntry1 = {
      ROOT_NODE, TEST_MH_LQE, index };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
#elif (MY_NODE_ID == NODE_ID_4)
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      NODE_ID_3, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_3, &index, RADIO_ID_915, FALSE);
   struct routingEntry routingEntry1 = {
      ROOT_NODE, TEST_MH_LQE, index };
   struct routingEntry routingEntry2 = {
      NODE_ID_2, TEST_MH_LQE, index };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
   network_insert((union networkEntry*)&routingEntry2, ROUTING_ENTRY);
#endif
#elif (TEST_ID == 0x3)
   uint8_t index;
#if (MY_NODE_ID == NODE_ID_4)
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      ROOT_NODE, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   struct neighborEntry neighborEntry2 = {
      NODE_ID_5, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   network_insert((union networkEntry*)&neighborEntry2, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_5, &index, RADIO_ID_915, FALSE);
   struct routingEntry routingEntry1 = {
      ROOT_NODE, TEST_MH_LQE, index };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
#elif (MY_NODE_ID == NODE_ID_5)
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      ROOT_NODE, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   struct neighborEntry neighborEntry2 = {
      NODE_ID_4, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   network_insert((union networkEntry*)&neighborEntry2, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_4, &index, RADIO_ID_915, FALSE);
   struct routingEntry routingEntry1 = {
      ROOT_NODE, TEST_MH_LQE, index };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
#endif
#endif
}

void loop()
{
#if (TEST_ID == 0x1)
#if MY_NODE_ID == ROOT_NODE
   //Form a test packet
   application_form_packet(txAppPacket, &txAttr, CMD_READ_REG, 55, 0, NULL);
   link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, ROOT_NODE, NODE_ID_3, ROOT_NODE, NODE_ID_2);
   //Send to destination
   dogeBool success = reliable_transmit();
   print_string("S:", NONE);
   print_decimal(success, NEWLINE);
#endif
#elif (TEST_ID == 0x2)
#if MY_NODE_ID == ROOT_NODE
   //Form a test packet
   application_form_packet(txAppPacket, &txAttr, CMD_WRITE_REG, NRF24_NODE_14_STATIC_ADDRESS, 0x55, NULL);
   link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, ROOT_NODE, NODE_ID_4, ROOT_NODE, NODE_ID_2);
   //Send to destination
   dogeBool success = reliable_transmit();
   print_string("S:", NONE);
   print_decimal(success, NEWLINE);
#endif
#endif
   uint8_t neighborIndex = 0;
   toggle_led(FALSE);

   //Make sure radio is ready to receive
   while (Radio.busy());

   // Turn on the receiver and listen for incoming data. Timeout after 500ms.
   if (reliable_receive(TIMEOUT_500_MS)){
      if(MY_NODE_ID == rxPacket.hdr.dst && MY_NODE_ID == rxPacket.hdr.shDst){ //parse message
         toggle_led(TRUE);
         sendResponse = link_layer_parse_packet(&spiProtocol, &rxPacket, &txPacket);
         //update neighbor table
         network_update(rxPacket.hdr.shSrc, Radio.getRssi(), RADIO_ID_915, NETWORK_ID_0, NEIGHBOR_ENTRY);
         if (sendResponse == TRANSMIT_RESPONSE){
            if (HEADER_TYPE_EQUALS(txPacket.hdr.type, RAW_PACKET)){
               reliable_transmit();
            }
         }
      }
      else if (MY_NODE_ID == rxPacket.hdr.shDst && MY_NODE_ID != rxPacket.hdr.dst){
         //      print_string("Forwarding attempt...", NONE);
         //      print_packet(&rxPacket);
         if(network_has_neighbor(rxPacket.hdr.dst, &tempIndex, RADIO_ID_915, FALSE)){
            if (HEADER_TYPE_EQUALS(rxPacket.hdr.type, RAW_PACKET)){
               toggle_led(TRUE);
               txAttr.ack = GET_HEADER_TYPE_ACK(rxPacket.hdr.type);
               txAttr.size = RAW_PACKET_DATA_SIZE(&rxPacket);
               copy_raw_packet_data((rawPacket*)&txPacket, (rawPacket*)&rxPacket);
               link_layer_form_packet(&txPacket, &txAttr, GET_HEADER_TYPE(rxPacket.hdr.type),
                                      rxPacket.hdr.src, rxPacket.hdr.dst, MY_NODE_ID,
                                      network[tempIndex].neighbor.shNodeID);
               reliable_transmit();
               /*print_string("Neighbor forward...", NONE);*/
               /*print_packet(&txPacket);*/
            }
         }
         else if (network_has_route(rxPacket.hdr.dst, &tempIndex, RADIO_ID_915, FALSE)){
            if (HEADER_TYPE_EQUALS(rxPacket.hdr.type, RAW_PACKET)){
               toggle_led(TRUE);
               txAttr.ack = GET_HEADER_TYPE_ACK(rxPacket.hdr.type);
               txAttr.size = RAW_PACKET_DATA_SIZE(&rxPacket);
               copy_raw_packet_data((rawPacket*)&txPacket, (rawPacket*)&rxPacket);
               neighborIndex = network[tempIndex].routing.neighborIndex;
               link_layer_form_packet(&txPacket, &txAttr, GET_HEADER_TYPE(rxPacket.hdr.type), 
                                      rxPacket.hdr.src, rxPacket.hdr.dst, MY_NODE_ID, 
                                      network[neighborIndex].neighbor.shNodeID);
               reliable_transmit();
               /*print_string("Route forward...", NONE);*/
               /*print_packet(&txPacket);*/
            }
         }
         else
         {
            Serial.println("ERROR: Attempt to forward packet that does not exist in neighbor or routing tables");
         }
      }
   }
   else{//end if got packet
      //no packet
   }

   if(dspStatus.counter == 1){ //reset counter and sample temperature sensor
      dspStatus.counter = dspStatus.period;
      dsp_add_sample( analogRead(A3) );    
   }
}

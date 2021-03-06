#include <stdint.h>
#include "dual_multihop.h"
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
#define TEST_ID 0x1
#define TEST_SH_LQE 0x1
#define TEST_MH_LQE 0x1

uint8_t rx_loop = 4;

// Data to write to radio TX FIFO (915 60 bytes MAX, nrf 32)  
dogePacket txPacket;
appPacket* txAppPacket;
packetAttr txAttr;

// Data to read from radio RX FIFO (915 60 bytes MAX, nrf 32.)
dogePacket rxPacket;
appPacket* rxAppPacket;
packetAttr rxAttr;

//nRF address
uint8_t nrf_address[5] = {
  0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t nrfRXAttempts = 0;
uint8_t nrfCounter = 0;
uint8_t ledValue = 0;

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

   //set up radios

   //nRF24L01P - 2.4GHz
#ifdef DUAL_RADIO
   pinMode(RADIO_NRF_CE, OUTPUT);
   digitalWrite(RADIO_NRF_CE, LOW);     
   pinMode(RADIO_NRF_CSN, OUTPUT);
   digitalWrite(RADIO_NRF_CSN, HIGH); 
#endif

   //CC1101L - 915MHz
   Radio.begin(ADDRESS_BROADCAST, CHANNEL_1, POWER_MAX); 

   //config nRF
#ifdef DUAL_RADIO
   SPI.begin(); 
   nrf24_init();
   nrf24_config(2, MAX_DATA_LENGTH);
   nrf24_tx_address(nrf_address);
   nrf24_rx_address(nrf_address);  
#endif


   pinMode(RED_LED, OUTPUT);
   digitalWrite(RED_LED, hbt_output);   // set the LED on

#if (TEST_ID == 0x1)
   uint8_t index;
#if (MY_NODE_ID == ROOT_NODE)
   rx_loop=2;
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      NODE_ID_2, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0  };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_2, &index, RADIO_ID_915, FALSE); 
   struct routingEntry routingEntry1 = {
      NODE_ID_3, TEST_MH_LQE, index  };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
#elif (MY_NODE_ID == NODE_ID_2)
   rx_loop=1;
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      ROOT_NODE, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0  };
   struct neighborEntry neighborEntry2 = {
      NODE_ID_3, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0  };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   network_insert((union networkEntry*)&neighborEntry2, NEIGHBOR_ENTRY);
#elif (MY_NODE_ID == NODE_ID_3)
   rx_loop=1;
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      NODE_ID_2, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0  };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_2, &index, RADIO_ID_915, FALSE); 
   struct routingEntry routingEntry1 = {
      ROOT_NODE, TEST_MH_LQE, index  };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
#endif
#elif (TEST_ID == 0x2)
   uint8_t index;
#if (MY_NODE_ID == ROOT_NODE)
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      NODE_ID_2, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0  };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_2, &index);
   struct routingEntry routingEntry1 = {
      NODE_ID_3, TEST_MH_LQE, index  };
   struct routingEntry routingEntry2 = {
      NODE_ID_4, TEST_MH_LQE, index  };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
   network_insert((union networkEntry*)&routingEntry2, ROUTING_ENTRY);
#elif (MY_NODE_ID == NODE_ID_2)
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      ROOT_NODE, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0  };
   struct neighborEntry neighborEntry2 = {
      NODE_ID_3, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0  };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   network_insert((union networkEntry*)&neighborEntry2, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_3, &index);
   struct routingEntry routingEntry1 = {
      NODE_ID_4, TEST_MH_LQE, index  };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
#elif (MY_NODE_ID == NODE_ID_3)
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      NODE_ID_2, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0  };
   struct neighborEntry neighborEntry2 = {
      NODE_ID_4, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0  };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   network_insert((union networkEntry*)&neighborEntry2, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_2, &index, ); 
   struct routingEntry routingEntry1 = {
      ROOT_NODE, TEST_MH_LQE, index  };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
#elif (MY_NODE_ID == NODE_ID_4)
   rx_loop=1;
   //Neighbor Table
   struct neighborEntry neighborEntry1 = {
      NODE_ID_3, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0  };
   network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
   //Routing Table
   network_has_neighbor(NODE_ID_3, &index);
   struct routingEntry routingEntry1 = {
      ROOT_NODE, TEST_MH_LQE, index  };
   struct routingEntry routingEntry2 = {
      NODE_ID_2, TEST_MH_LQE, index  };
   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
   network_insert((union networkEntry*)&routingEntry2, ROUTING_ENTRY);
#endif
#endif

   uint8_t i = 0;
}

void loop()
{
#if (TEST_ID == 0x1)
#if MY_NODE_ID == ROOT_NODE
   memset(&txPacket, 0, sizeof(dogePacket));
   memset(&rxPacket, 0, sizeof(dogePacket));
   txAppPacket = (appPacket*)((uint8_t*)&txPacket + RAW_PACKET_DATA_OFFSET);
   rxAppPacket = (appPacket*)((uint8_t*)&rxPacket + RAW_PACKET_DATA_OFFSET);
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
   memset(&txPacket, 0, sizeof(dogePacket));
   memset(&rxPacket, 0, sizeof(dogePacket));
   txAppPacket = (appPacket*)((uint8_t*)&txPacket + RAW_PACKET_DATA_OFFSET);
   rxAppPacket = (appPacket*)((uint8_t*)&rxPacket + RAW_PACKET_DATA_OFFSET);
   //Form a test packet
   application_form_packet(txAppPacket, &txAttr, CMD_READ_REG, 55, 0, NULL);
   link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, ROOT_NODE, NODE_ID_4, ROOT_NODE, NODE_ID_2);
   //Send to destination
   dogeBool success = reliable_transmit();
   print_string("S:", NONE);
   print_decimal(success, NEWLINE);
#endif
#endif
   toggle_led(FALSE);

   //Make sure radio is ready to receive
   while (Radio.busy());
#ifdef DUAL_RADIO
   if(nrfCounter > 0){
      nrfCounter = 0;
      memset(&txPacket, 0, sizeof(dogePacket));
      memset(&rxPacket, 0, sizeof(dogePacket));
      txAppPacket = (appPacket*)((uint8_t*)&txPacket + RAW_PACKET_DATA_OFFSET);
      rxAppPacket = (appPacket*)((uint8_t*)&rxPacket + RAW_PACKET_DATA_OFFSET);
      //Form a test packet
      application_form_packet(txAppPacket, &txAttr, CMD_WRITE_REG, 4, ledValue++, NULL);
      link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, MY_NODE_ID, 11, MY_NODE_ID, 11);  

      SPI.begin();
      nrf24_send(0, (uint8_t*)(&txPacket), MAX_DATA_LENGTH);
      while(nrf24_isSending());
   }
#endif
   // Turn on the receiver and listen for incoming data. Timeout after 500ms.
   if (reliable_receive(TIMEOUT_500_MS)){
      nrfCounter++;
      if(MY_NODE_ID == rxPacket.hdr.dst && MY_NODE_ID == rxPacket.hdr.shDst){ //parse message   
         toggle_led(TRUE);
         sendResponse = link_layer_parse_packet(&spiProtocol, &rxPacket, &txPacket);
         //update neighbor table
         network_update(rxPacket.hdr.shSrc, Radio.getRssi(), RADIO_ID_915, NETWORK_ID_0, NEIGHBOR_ENTRY);
         if (sendResponse == TRANSMIT_RESPONSE){ // -- this should be done by node 3
            if (HEADER_TYPE_EQUALS(txPacket.hdr.type, RAW_PACKET)){
               reliable_transmit();
            }
         }
      }
      else if (MY_NODE_ID == rxPacket.hdr.shDst && MY_NODE_ID != rxPacket.hdr.dst){ //forward message
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
               //          print_string("Forwarded...", NONE);
               //          print_packet(&txPacket);
            }
         }
         else if (network_has_route(rxPacket.hdr.dst, &tempIndex, RADIO_ID_915, FALSE)){
            if (HEADER_TYPE_EQUALS(rxPacket.hdr.type, RAW_PACKET)){
               toggle_led(TRUE);
               txAttr.ack = GET_HEADER_TYPE_ACK(rxPacket.hdr.type);
               txAttr.size = RAW_PACKET_DATA_SIZE(&rxPacket);
               copy_raw_packet_data((rawPacket*)&txPacket, (rawPacket*)&rxPacket);
               link_layer_form_packet(&txPacket, &txAttr, GET_HEADER_TYPE(rxPacket.hdr.type), 
                                      rxPacket.hdr.src, rxPacket.hdr.dst, MY_NODE_ID, 
                                      network[tempIndex].neighbor.shNodeID);
               reliable_transmit();
               //print_string("Forwarded...", NONE);
               //print_packet(&txPacket);
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

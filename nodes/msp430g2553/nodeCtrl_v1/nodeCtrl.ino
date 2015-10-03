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
#if 0
   digitalWrite(RED_LED, LOW);   // set the LED off
#else
   digitalWrite(RED_LED, HIGH);   // set the LED on
#endif
#if (DEMO_GRID == 1)
   uint8_t index;
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
#endif
}

void loop()
{
   uint8_t neighborIndex = 0;
   toggle_led(FALSE);

   //Make sure radio is ready to receive
   while (Radio.busy());

   // Turn on the receiver and listen for incoming data. Timeout after 100ms.
   if (reliable_receive(TIMEOUT_100_MS)){
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

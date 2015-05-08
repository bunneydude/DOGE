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

/*******************************************************************************
 * Test 1
 * Node 1 uses hard coded neighbor tables/routing tables to pull a network
 * table from the furthest physical node (Node 3). 
 * Node 3 has 4 neighbors (2,4,5,6). 4,5,6 are not physically present.
 * Node 3 has 4 routes (Root through 2, 4 through 5, 7 through 5, 7 through 4)
 *******************************************************************************/

/*******************************************************************************
 * Test 2
 * Node 1 uses hard coded neighbor tables/routing tables to send a packet to the
 * furthest node (Node 4).
 *******************************************************************************/
#define TEST_ID 0x1
#define TEST_SH_LQE 0x1
#define TEST_MH_LQE 0x1

// Data to write to radio TX FIFO (60 bytes MAX.)
//unsigned char txData[DATA_LENGTH];    
dogePacket txPacket;
appPacket* txAppPacket;
packetAttr txAttr;

// Data to read from radio RX FIFO (60 bytes MAX.)
//unsigned char rxData[DATA_LENGTH];
dogePacket rxPacket;
appPacket* rxAppPacket;
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
  network_has_neighbor(NODE_ID_2, &index); 
  struct routingEntry routingEntry1 = {
    NODE_ID_3, TEST_MH_LQE, index };
  network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
#elif (MY_NODE_ID == NODE_ID_2)
  //Neighbor Table
  struct neighborEntry neighborEntry1 = {
    ROOT_NODE, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
  struct neighborEntry neighborEntry2 ={
    NODE_ID_3, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
  network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
  network_insert((union networkEntry*)&neighborEntry2, NEIGHBOR_ENTRY);
#elif (MY_NODE_ID == NODE_ID_3)
  //Neighbor Table
  struct neighborEntry neighborEntry1 = {
    NODE_ID_2, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
  struct neighborEntry neighborEntry2 = {
    NODE_ID_4, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
  struct neighborEntry neighborEntry3 = {
    NODE_ID_5, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
  struct neighborEntry neighborEntry4 = {
    NODE_ID_6, TEST_SH_LQE, RADIO_ID_915, NETWORK_ID_0 };
  network_insert((union networkEntry*)&neighborEntry1, NEIGHBOR_ENTRY);
  network_insert((union networkEntry*)&neighborEntry2, NEIGHBOR_ENTRY);
  network_insert((union networkEntry*)&neighborEntry3, NEIGHBOR_ENTRY);
  network_insert((union networkEntry*)&neighborEntry4, NEIGHBOR_ENTRY);
  //Routing Table
  network_has_neighbor(NODE_ID_2, &index);
  struct routingEntry routingEntry1 = {
    ROOT_NODE, TEST_MH_LQE, index };
  network_has_neighbor(NODE_ID_5, &index);
  struct routingEntry routingEntry2 = {
    NODE_ID_4, TEST_MH_LQE, index };
  network_has_neighbor(NODE_ID_5, &index);
  struct routingEntry routingEntry3 = {
    NODE_ID_7, TEST_MH_LQE, index };
  network_has_neighbor(NODE_ID_4, &index);
  struct routingEntry routingEntry4 = {
    NODE_ID_7, TEST_MH_LQE, index };
  network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
  network_insert((union networkEntry*)&routingEntry2, ROUTING_ENTRY);
  network_insert((union networkEntry*)&routingEntry3, ROUTING_ENTRY);
  network_insert((union networkEntry*)&routingEntry4, ROUTING_ENTRY);
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
  network_has_neighbor(NODE_ID_2, &index); 
  struct routingEntry routingEntry1 = {
    ROOT_NODE, TEST_MH_LQE, index  };
  network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
#elif (MY_NODE_ID == NODE_ID_4)
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
}

void loop()
{
#if (TEST_ID == 0x1)
#if MY_NODE_ID == ROOT_NODE
  memset(&txPacket, 0, sizeof(dogePacket));
  memset(&rxPacket, 0, sizeof(dogePacket));
  txAppPacket = (appPacket*)((uint8_t*)&txPacket + RAW_PACKET_DATA_OFFSET);
  rxAppPacket = (appPacket*)((uint8_t*)&rxPacket + RAW_PACKET_DATA_OFFSET);
  //Read first 12 bytes
  application_form_packet(txAppPacket, &txAttr, CMD_READ_MEM, MM_NETWORK_BASE, MAX_CMD_READ_MEM_DATA_SIZE);
  link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, ROOT_NODE, NODE_ID_3, ROOT_NODE, NODE_ID_2);
  //Send to destination
  dogeBool success = reliable_transmit();
  print_string("S:", NONE);
  print_decimal(success, NEWLINE);
  if (!reliable_receive(TIMEOUT_1000_MS)){
    return;
  }
  print_packet(&rxPacket);
  //Read second 12 bytes
  application_form_packet(txAppPacket, &txAttr, CMD_READ_MEM, MM_NETWORK_BASE + MAX_CMD_READ_MEM_DATA_SIZE, MAX_CMD_READ_MEM_DATA_SIZE);
  link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, ROOT_NODE, NODE_ID_3, ROOT_NODE, NODE_ID_2);
  //Send to destination
  success = reliable_transmit();
  print_string("S:", NONE);
  print_decimal(success, NEWLINE);
  if (!reliable_receive(TIMEOUT_1000_MS)){
    return;
  }
  print_packet(&rxPacket);
  //Read third 12 bytes
  application_form_packet(txAppPacket, &txAttr, CMD_READ_MEM, MM_NETWORK_BASE + 2*MAX_CMD_READ_MEM_DATA_SIZE, 8);
  link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, ROOT_NODE, NODE_ID_3, ROOT_NODE, NODE_ID_2);
  //Send to destination
  success = reliable_transmit();
  print_string("S:", NONE);
  print_decimal(success, NEWLINE);
  if (!reliable_receive(TIMEOUT_1000_MS)){
    return;
  }
  print_packet(&rxPacket);

#if 0
  //Overwrite last 2 entries in network table
  memset(&txPacket, 0, sizeof(dogePacket));
  application_form_packet(txAppPacket, &txAttr, CMD_WRITE_MEM, MM_NETWORK_BASE + 2*MAX_CMD_READ_MEM_DATA_SIZE, 2*sizeof(struct routingEntry));
  copy_bytes(CMD_WRITE_MEM_DATA_ADDRESS(txAppPacket), CMD_READ_MEM_DATA_ADDRESS(rxAppPacket), 2*sizeof(struct routingEntry));
  memset(CMD_WRITE_MEM_DATA_ADDRESS(txAppPacket), 0, 8);
  link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, ROOT_NODE, NODE_ID_3, ROOT_NODE, NODE_ID_2);
  print_string("WRITING...", NEWLINE); print_packet(&txPacket);
  //Send to destination
  success = reliable_transmit();
  print_string("S:", NONE);
  print_decimal(success, NEWLINE);
  if (!reliable_receive(TIMEOUT_1000_MS)){
    return;
  }
  print_packet(&rxPacket);
#endif

#endif
#elif (TEST_ID == 0x2)
#if MY_NODE_ID == ROOT_NODE
  memset(&txPacket, 0, sizeof(dogePacket));
  memset(&rxPacket, 0, sizeof(dogePacket));
  txAppPacket = (appPacket*)((uint8_t*)&txPacket + RAW_PACKET_DATA_OFFSET);
  rxAppPacket = (appPacket*)((uint8_t*)&rxPacket + RAW_PACKET_DATA_OFFSET);
  //Form a test packet
  application_form_packet(txAppPacket, &txAttr, CMD_READ_REG, 55, 0);
  link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, ROOT_NODE, NODE_ID_4, ROOT_NODE, NODE_ID_2);
  //Send to destination
  dogeBool success = reliable_transmit();
  print_string("S:", NONE);
  print_decimal(success, NEWLINE);
#endif
#endif

  //for(i=0; i<rx_loop; i++){
    //Make sure radio is ready to receive
    while (Radio.busy());

  // Turn on the receiver and listen for incoming data. Timeout after 1000ms.
  if (reliable_receive(TIMEOUT_1000_MS)){
      if(MY_NODE_ID == rxPacket.hdr.dst && MY_NODE_ID == rxPacket.hdr.shDst){ //parse message   
        digitalWrite(RED_LED, hbt_output ^= 0x1);
        //parse message
        sendResponse = link_layer_parse_packet(&spiProtocol, &rxPacket, &txPacket);
        //update neighbor table
        network_update(rxPacket.hdr.shSrc, Radio.getRssi(), RADIO_ID_915, NETWORK_ID_0, NEIGHBOR_ENTRY);
        if (sendResponse == TRANSMIT_RESPONSE){ // -- this should be done by node 3
          if (HEADER_TYPE_EQUALS(txPacket.hdr.type, RAW_PACKET)){
            reliable_transmit();
          }
        }
      }
      else if (MY_NODE_ID == rxPacket.hdr.shDst && MY_NODE_ID != rxPacket.hdr.dst){ //forward message --- This is done by node 2
        //      print_string("Forwarding attempt...", NONE);
        //      print_packet(&rxPacket);
        if(network_has_neighbor(rxPacket.hdr.dst, &tempIndex)){
          if (HEADER_TYPE_EQUALS(rxPacket.hdr.type, RAW_PACKET)){
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
        else if (network_has_route(rxPacket.hdr.dst, &tempIndex)){
          if (HEADER_TYPE_EQUALS(rxPacket.hdr.type, RAW_PACKET)){
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
//  }

  if(dspStatus.counter == 1){ //reset counter and sample temperature sensor
    dspStatus.counter = dspStatus.period;
    dsp_add_sample( analogRead(A3) );    
  }
}

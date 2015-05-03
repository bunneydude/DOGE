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
// -----------------------------------------------------------------------------
/**
 *  Global data
 */
#define MAX_DATA_LENGTH 32
#define MY_NODE_ID 0x6
#define NODECTRL_VERSION 1
//Defines for network/radio IDs
#define RADIO_ID_915 (0x1)
#define NETWORK_ID_0 (0x0)
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

}

void loop()
{  
  //Make sure radio is ready to receive
  while (Radio.busy());

  // Turn on the receiver and listen for incoming data. Timeout after 1 seconds.
  if (Radio.receiverOn((uint8_t*)(&rxPacket), MAX_DATA_LENGTH, 1000) > 0){
    if(MY_NODE_ID == rxPacket.hdr.dst && MY_NODE_ID == rxPacket.hdr.shDst){ //parse message
      digitalWrite(RED_LED, hbt_output ^= 0x1);
      //parse message
      sendResponse = link_layer_parse_packet(&spiProtocol, &rxPacket, &txPacket);
      //update neighbor table
      network_update(rxPacket.hdr.shSrc, Radio.getRssi(), RADIO_ID_915, NETWORK_ID_0, NEIGHBOR_ENTRY);
      if (sendResponse == TRANSMIT_RESPONSE){
        if (HEADER_TYPE_EQUALS(txPacket.hdr.type, RAW_PACKET)){
          Radio.transmit(ADDRESS_BROADCAST, (uint8_t*)(&txPacket), RAW_PACKET_TOTAL_SIZE(&txPacket));
        }
      }
    }
    else if (MY_NODE_ID == rxPacket.hdr.shDst && MY_NODE_ID != rxPacket.hdr.dst){ //forward message
      if(network_has_neighbor(rxPacket.hdr.dst, &tempIndex)){
        if (HEADER_TYPE_EQUALS(rxPacket.hdr.type, RAW_PACKET)){
          txAttr.ack = GET_HEADER_TYPE_ACK(rxPacket.hdr.type);
          txAttr.size = RAW_PACKET_DATA_SIZE(&rxPacket);
          copy_raw_packet_data((rawPacket*)&txPacket, (rawPacket*)&rxPacket);
          link_layer_form_packet(&txPacket, &txAttr, GET_HEADER_TYPE(rxPacket.hdr.type),
                                 rxPacket.hdr.src, rxPacket.hdr.dst, MY_NODE_ID,
                                 network[tempIndex].neighbor.shNodeID);
          Radio.transmit(ADDRESS_BROADCAST, (uint8_t*)(&txPacket), RAW_PACKET_TOTAL_SIZE(&txPacket));
        }
      }
      else if (network_has_route(rxPacket.hdr.dst, &tempIndex)){
        if (HEADER_TYPE_EQUALS(txPacket.hdr.type, RAW_PACKET)){
          txAttr.ack = GET_HEADER_TYPE_ACK(rxPacket.hdr.type);
          txAttr.size = RAW_PACKET_DATA_SIZE(&rxPacket);
          copy_raw_packet_data((rawPacket*)&txPacket, (rawPacket*)&rxPacket);
          link_layer_form_packet(&txPacket, &txAttr, GET_HEADER_TYPE(rxPacket.hdr.type), 
                                 rxPacket.hdr.src, rxPacket.hdr.dst, MY_NODE_ID, 
                                 network[tempIndex].neighbor.shNodeID);
          Radio.transmit(ADDRESS_BROADCAST, (uint8_t*)(&txPacket), RAW_PACKET_TOTAL_SIZE(&txPacket));
        }
      }
      else
      {
        Serial.println("ERROR: Attempt to forward packet that does not exist in neighbor or routing tables");
      }
    }
  }
  else{//end if got packet
    //we timed out, decrement counter...this is lame I know
    // TODO use the Energia library to set up a timer ISR
    if(dspStatus.counter > 1){
      dspStatus.counter--;
    }
  }

  if(dspStatus.counter == 1){ //reset counter and sample temperature sensor
    dspStatus.counter = dspStatus.period;
    dsp_add_sample( analogRead(A3) );    
  }
}

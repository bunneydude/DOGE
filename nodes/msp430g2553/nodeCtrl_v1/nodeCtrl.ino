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
rawPacket txRawPacket;
appPacket* txAppPacket;
packetAttr txAttr;

// Data to read from radio RX FIFO (60 bytes MAX.)
//unsigned char rxData[DATA_LENGTH];
rawPacket rxRawPacket;
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

  memset(&txRawPacket, 0, sizeof(rawPacket));
  memset(&rxRawPacket, 0, sizeof(rawPacket));  
  txAppPacket = (appPacket*)txRawPacket.data;
  rxAppPacket = (appPacket*)rxRawPacket.data;

  application_form_packet(txAppPacket, &txAttr, CMD_READ_REG, 55, 0);
  link_layer_form_packet(&txRawPacket, &txAttr, RAW_PACKET, 1, 6, 1, 6); //TODO Mark please take a look

  sendResponse = link_layer_parse_packet(&spiProtocol, &txRawPacket, &rxRawPacket);

  i=0;
  sendResponse = 1;
}

void loop()
{  
  //Make sure radio is ready to receive
  while (Radio.busy());

  // Turn on the receiver and listen for incoming data. Timeout after 1 seconds.
  if (Radio.receiverOn((uint8_t*)(&rxRawPacket), MAX_DATA_LENGTH, 1000) > 0){
    if(MY_NODE_ID == rxRawPacket.hdr.dst){ //parse message
      digitalWrite(RED_LED, hbt_output ^= 0x1);
      //parse message
      sendResponse = link_layer_parse_packet(&spiProtocol, &rxRawPacket, &txRawPacket);
      //update neighbor table
      network_update(rxRawPacket.hdr.shSrc, Radio.getRssi(), RADIO_ID_915, NETWORK_ID_0, NEIGHBOR_ENTRY);
      if (sendResponse == TRANSMIT_RESPONSE){
        Radio.transmit(ADDRESS_BROADCAST, (uint8_t*)(&txRawPacket), sizeof(packetHdr) + 1 + txRawPacket.size); //to root node at address 0x1
      }
    }
    else if (MY_NODE_ID == rxRawPacket.hdr.shDst){ //forward message
      if(network_has_neighbor(rxRawPacket.hdr.shDst, &tempIndex)){
         txAttr.ack = GET_HEADER_TYPE_ACK(rxRawPacket.hdr.type);
         txAttr.size = rxRawPacket.size;
         link_layer_form_packet(&txRawPacket, &txAttr, GET_HEADER_TYPE(rxRawPacket.hdr.type), rxRawPacket.hdr.src, rxRawPacket.hdr.dst,
                                MY_NODE_ID, network[tempIndex].neighbor.shNodeID);
         Radio.transmit(ADDRESS_BROADCAST, (uint8_t*)(&txRawPacket), sizeof(packetHdr) + 1 + txRawPacket.size); //to root node at address 0x1
      }
      else if (network_has_route(rxRawPacket.hdr.shDst, &tempIndex)){
         txAttr.ack = GET_HEADER_TYPE_ACK(rxRawPacket.hdr.type);
         txAttr.size = rxRawPacket.size;
         link_layer_form_packet(&txRawPacket, &txAttr, GET_HEADER_TYPE(rxRawPacket.hdr.type), rxRawPacket.hdr.src, rxRawPacket.hdr.dst,
                                MY_NODE_ID, network[tempIndex].neighbor.shNodeID);
         Radio.transmit(ADDRESS_BROADCAST, (uint8_t*)(&txRawPacket), sizeof(packetHdr) + 1 + txRawPacket.size); //to root node at address 0x1
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

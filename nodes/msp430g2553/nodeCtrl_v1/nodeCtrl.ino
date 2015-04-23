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
// -----------------------------------------------------------------------------
/**
 *  Global data
 */
#define MAX_DATA_LENGTH 32
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
uint8_t typeAck;
packetType type;

#define MY_NODE_ID 0x6
#define NODECTRL_VERSION 1

struct Protocol spiProtocol;
uint8_t sendResponse = 0;
uint8_t hbt_output = 0x1;

uint8_t returnData = 0;
uint8_t i = 0;
uint8_t rawADC = 0;
uint8_t tempIndex;
union networkEntry tempEntry;
// -----------------------------------------------------------------------------
// Main example

void print_packet(rawPacket* packet){
  uint8_t i = 0;
  uint8_t* bytes = (uint8_t*)(packet);

  Serial.println("Packet");
  Serial.print("   ["); 
  for(i=0; i < (sizeof(packetHdr) + 1 + packet->size); i++){
    Serial.print(bytes[i]);
  }  
  Serial.println("]");
}

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
  Radio.begin(0x01, CHANNEL_1, POWER_MAX); 

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

      if(sendResponse == 1){
        Radio.transmit(0x1, (uint8_t*)(&txRawPacket), sizeof(packetHdr) + 1 + txRawPacket.size); //to root node at address 0x1
      }
    }
    else if (MY_NODE_ID == rxRawPacket.hdr.shDst &&
             network_has_neighbor(rxRawPacket.hdr.shDst, &tempIndex)){ //forward message
             //TODO NEED TO CHECK ROUTING TABLE AS WELL
      GET_HEADER_TYPE_ACK(rxRawPacket.hdr.type, typeAck);
      GET_HEADER_TYPE(rxRawPacket.hdr.type, type);
      txAttr.ack = typeAck;
      txAttr.size = rxRawPacket.size;
      link_layer_form_packet(&txRawPacket, &txAttr, type, rxRawPacket.hdr.src, rxRawPacket.hdr.dst,
                             MY_NODE_ID, network[tempIndex].neighbor.shNodeID);
      Radio.transmit(0x1, (uint8_t*)(&txRawPacket), sizeof(packetHdr) + 1 + txRawPacket.size); //to root node at address 0x1
    }
    //Received packet was not meant for us
    //check if this is a new neighbor
    if(network_has_neighbor(rxRawPacket.hdr.src, &tempIndex)){
      network[tempIndex].neighbor.shLQE = 0xFF & Radio.getRssi();
      Serial.print("Neighbor "); Serial.print(rxRawPacket.hdr.src); 
      Serial.print(", RSSI 0x"); Serial.println(network[tempIndex].neighbor.shLQE, HEX);
    }else{ //make a new entry
      tempEntry.neighbor.shNodeID = rxRawPacket.hdr.src;
      tempEntry.neighbor.shLQE = 0xFF & Radio.getRssi();
      tempEntry.neighbor.radioID = 0x1;
      tempEntry.neighbor.networkID = 0x0;

      returnData = network_insert(&tempEntry, NEIGHBOR_ENTRY); 
      if(returnData == 1){
        Serial.print("Added neighbor: "); 
        Serial.println(rxRawPacket.hdr.src);
      }
      else{
        Serial.print("No room for neighbor: "); 
        Serial.println(rxRawPacket.hdr.src);
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

}//end main

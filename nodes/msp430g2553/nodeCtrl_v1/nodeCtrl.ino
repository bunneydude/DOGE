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

}

void loop()
{  
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
      if (sendResponse == TRANSMIT_RESPONSE){
        if (HEADER_TYPE_EQUALS(txPacket.hdr.type, RAW_PACKET)){
          reliable_transmit();
        }
      }
    }
    else if (MY_NODE_ID == rxPacket.hdr.shDst && MY_NODE_ID != rxPacket.hdr.dst){
      if(network_has_neighbor(rxPacket.hdr.dst, &tempIndex, RADIO_ID_915, FALSE)){
        if (HEADER_TYPE_EQUALS(rxPacket.hdr.type, RAW_PACKET)){
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
          txAttr.ack = GET_HEADER_TYPE_ACK(rxPacket.hdr.type);
          txAttr.size = RAW_PACKET_DATA_SIZE(&rxPacket);
          copy_raw_packet_data((rawPacket*)&txPacket, (rawPacket*)&rxPacket);
          link_layer_form_packet(&txPacket, &txAttr, GET_HEADER_TYPE(rxPacket.hdr.type), 
              rxPacket.hdr.src, rxPacket.hdr.dst, MY_NODE_ID, 
              network[tempIndex].neighbor.shNodeID);
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

#include "doge_node.h"
#include <SPI.h>
#include <AIR430BoostFCC.h>
#include <Protocol.h>
#include <MyRingBuffer.h>
#include <stdint.h>

// -----------------------------------------------------------------------------
/**
 *  Global data
 */

// Data to write to radio TX FIFO (60 bytes MAX.)
unsigned char txData[7];    

// Data to read from radio RX FIFO (60 bytes MAX.)
unsigned char rxData[7];

uint8_t nodeID = 0x5;
Protocol myProtocol(0);
uint8_t sent = 0;
uint8_t sendResponse = 0;
uint8_t hbt_output = 0x1;
// -----------------------------------------------------------------------------
// Main example

void setup()
{
  
  myProtocol.serial_registers[SREG_NODE_ID] = nodeID;
  myProtocol.serial_registers[SREG_TX_PWR] = 0;
  myProtocol.serial_registers[SREG_RSSI] = 0;
  myProtocol.serial_registers[SREG_LQI] = 0;
  myProtocol.serial_registers[SREG_TEMPERATURE] = 0;
  
  Serial.begin(9600);
  
  // The radio library uses the SPI library internally, this call initializes
  // SPI/CSn and GDO0 lines. Also setup initial address, channel, and TX power.
  Radio.begin(0x02, CHANNEL_1, POWER_MAX); 

  /**
   *  Setup LED for example demonstration purposes.
   *
   *  Note: Set radio first to ensure that GDO2 line isn't being driven by the 
   *  MCU as it is an output from the radio.
   */
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, hbt_output);   // set the LED on
}

void loop()
{  
  //Make sure radio is ready
  while (Radio.busy());
  
  // Turn on the receiver and listen for incoming data. Timeout after 1 seconds.
  if (Radio.receiverOn(rxData, sizeof(rxData), 1000) > 0)
  {
    if(rxData[1] == nodeID){
      digitalWrite(RED_LED, hbt_output ^= 0x1);

      //update radio info
      myProtocol.serial_registers[SREG_RSSI] = Radio.getRssi();
      myProtocol.serial_registers[SREG_LQI] = Radio.getLqi();
//      myProtocol.serial_registers[SREG_TEMPERATURE] = 42;//(analogRead(A3) >> 2)&0xFF;

      uint16_t rawTemp = analogRead(A3);
      myProtocol.serial_registers[SREG_TEMPERATURE] = rawTemp >> 2;

      Serial.print("ADC: ");
      Serial.write(myProtocol.serial_registers[SREG_TEMPERATURE]);

      //parse message
      sendResponse = myProtocol.parse_packet(&rxData[2], &txData[2]);
    
      txData[0] = nodeID;
      txData[1] = rxData[0];      
    
      if(sendResponse == 1){
        Radio.transmit(0x1, txData, 7); //to root node at address 0x1
      }
      
      //Update settings
      //TODO, channel, power, nodeID
    }   
  }
}

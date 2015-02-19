#include "radio_proxy.h"
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

uint8_t nodeID = 0x1;

Protocol myProtocol(0);
uint8_t sent = 0;
uint8_t i = 1;
uint8_t hbt_output = 0x0;
// -----------------------------------------------------------------------------
// Main example

void setup()
{
  
  myProtocol.serial_registers[SREG_NODE_ID] = nodeID;
  myProtocol.serial_registers[SREG_TX_PWR] = 0;
  myProtocol.serial_registers[SREG_RSSI] = 0;
  myProtocol.serial_registers[SREG_LQI] = 0;
  myProtocol.serial_registers[SREG_TEMPERATURE] = 0;
  
  // The radio library uses the SPI library internally, this call initializes
  // SPI/CSn and GDO0 lines. Also setup initial address, channel, and TX power.
  Radio.begin(0x01, CHANNEL_1, POWER_MAX);

  Serial.begin(9600);
  
  uint8_t temp;
   while(Serial.available()){ //flush
      temp = Serial.read();
   }
   

  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, hbt_output);
}

void loop(){
  
  sent = serial_rf_fetch();

  if(myProtocol.serial_registers[SREG_PING] == 0x1){
    myProtocol.serial_registers[SREG_PING] = 0;
    
    //read rssi
    myProtocol.form_packet(&txData[2], READ_REG, SREG_RSSI, 0); //payload
    
    //header
    txData[0] = nodeID;
    txData[1] = myProtocol.serial_registers[SREG_TARGET];
    
    //transmit
    Radio.transmit(0x2, txData, 7);
    
    //Make sure radio is ready
    while (Radio.busy());

    // Turn on the receiver and listen for incoming data. Timeout after 1 seconds.
    if (Radio.receiverOn(rxData, sizeof(rxData), 1000) > 0){
      digitalWrite(RED_LED, hbt_output ^= 0x1);

      //update radio info
      myProtocol.serial_registers[SREG_RSSI] = Radio.getRssi();
      myProtocol.serial_registers[SREG_LQI] = Radio.getLqi();

    }else{ //timeout, send error message
      myProtocol.form_packet(&rxData[2], NACK, 0, TIMEOUT);     
    }
    //send result to Edison
      for(i=0; i<rxData[2+1]+3; i++){
        myProtocol._uartTxBuffer.write(rxData[2+i]);
      }
      myProtocol.uartTx();                    
  }
}

uint8_t serial_rf_fetch(){
   if(Serial.available()){
      uint8_t rxBuffer[5];
      uint8_t response[5];   
      uint8_t index = 0;
      uint8_t i = 0;
      uint8_t sendResponse = 0;

      while(Serial.available() < 2); //wait for 2 bytes
      rxBuffer[index++] = Serial.read(); //cmd
      rxBuffer[index++] = Serial.read(); //size

      for(i=0; i<rxBuffer[1]; i++){ //get rest of bytes
         while(Serial.available() == 0); //wait for a byte
         rxBuffer[index++] = Serial.read();
      }

      while(Serial.available() == 0); //wait for checksum
      rxBuffer[index++] = Serial.read();


      //parse packet
      sendResponse = myProtocol.parse_packet(rxBuffer, response);

      if( (sendResponse == 1) && (myProtocol.serial_registers[SREG_PING] == 0x0) ){
         for(i=0; i<response[1]+3; i++){
            myProtocol._uartTxBuffer.write(response[i]);
         }
         myProtocol.uartTx();
      }
      return 1;
   }
   return 0;
}




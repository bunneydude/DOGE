#include "proxy_nrf_only.h"
#include <SPI.h>
#include <nrf24.h>
#include <nRF24L01.h>
#include <Protocol.h>
#include <MyRingBuffer.h>
#include <stdint.h>

// -----------------------------------------------------------------------------
/**
 *  Global data
 */

// Data to write to radio TX FIFO (60 bytes MAX.)
uint8_t txData[PROXY_PAYLOAD];    

// Data to read from radio RX FIFO (60 bytes MAX.)
uint8_t rxData[PROXY_PAYLOAD];

uint8_t nodeID = 0x1;
uint8_t nrf_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t nrfRXAttempts = 0;

Protocol myProtocol(0);
uint8_t sent = 0;
uint8_t i = 1;
uint8_t hbt_output = 0x0;
// -----------------------------------------------------------------------------
//functions for nRF
void nrf24_ce_digitalWrite(uint8_t state){
      digitalWrite(RADIO_NRF_CE,state);
}

void nrf24_csn_digitalWrite(uint8_t state){
      digitalWrite(RADIO_NRF_CSN,state);
}



uint8_t spi_transfer(uint8_t tx)
{
    uint8_t rx = 0;    
    rx = SPI.transfer(tx);
    return rx;
}

uint8_t nrf_read_timeout(uint16_t timeout){
  while(timeout-- > 0){
    delay(1);
    if(nrf24_dataReady() != 0){
      return 1;
    }
  }
  return 0;    
  }

// Main example

void setup()
{
  pinMode(P2_7, OUTPUT);
  digitalWrite(P2_7, HIGH);
  
  pinMode(RADIO_NRF_CE, OUTPUT);
  digitalWrite(RADIO_NRF_CE, LOW);     
  pinMode(RADIO_NRF_CSN, OUTPUT);
  digitalWrite(RADIO_NRF_CSN, HIGH); 
  
  Serial.begin(9600);
  
  myProtocol.serial_registers[SREG_NODE_ID] = nodeID;
  myProtocol.serial_registers[SREG_TX_PWR] = 0;
  myProtocol.serial_registers[SREG_RSSI] = 0;
  myProtocol.serial_registers[SREG_LQI] = 0;
  myProtocol.serial_registers[SREG_TEMPERATURE] = 0;
  myProtocol.serial_registers[SREG_PING] = 2;
  myProtocol.serial_registers[SREG_TARGET] = 2;
  
  // The radio library uses the SPI library internally, this call initializes
  // SPI/CSn and GDO0 lines. Also setup initial address, channel, and TX power.

 SPI.begin(); 
  nrf24_init();
  nrf24_config(2,PROXY_PAYLOAD);
  nrf24_tx_address(nrf_address);
  nrf24_rx_address(nrf_address);  

  
  
  uint8_t temp;
   while(Serial.available()){ //flush rx pipe
      temp = Serial.read();
   }
   

  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, hbt_output);
}

void printRXBytes(){
  Serial.println("Got data:");
  for(uint8_t i=0; i<PROXY_PAYLOAD; i++){
   Serial.write(rxData[i]);
  Serial.print(", ");   
  }
}
uint8_t temperature = 0;

void loop(){
  
//  sent = serial_rf_fetch();

//  if(myProtocol.serial_registers[SREG_PING] != 0){
    
    //read temperature
    myProtocol.form_packet(&txData[2], WRITE_REG, SREG_TEMPERATURE, temperature);
    temperature += 8;
    
    //header
    // byte 0 = ID of source node
    // byte 1 = ID of destination node
    txData[0] = nodeID;
    txData[1] = myProtocol.serial_registers[SREG_TARGET];
    
    if(myProtocol.serial_registers[SREG_PING] == 0x2){
      nrf24_send(txData);
      while(nrf24_isSending());
      //TODO something w/ RT count here     
      nrf24_powerUpRx();
      
//      if(nrf_read_timeout(1000) == 1){//update radio info
      while(!nrf24_dataReady());
        digitalWrite(RED_LED, hbt_output ^= 0x1);
        nrf24_getData(rxData);
        printRXBytes();
        
//      }else{//send timeout message
//        myProtocol.form_packet(&rxData[2], NACK, 0, TIMEOUT);     
//      }
      
      // myProtocol.form_packet(&rxData[2], ACK, SREG_TEMPERATURE, temperature);     
    }else{ //unknown value in SREG_PING
      myProtocol.form_packet(&rxData[2], NACK, 0, BAD_COMMAND);     
    }    
//    myProtocol.serial_registers[SREG_PING] = 0x0; //reset
    /*
    //send result to Edison
    uint8_t payloadSize = rxData[2+1]+3;
    if(payloadSize > PROXY_PAYLOAD-2){
      payloadSize = PROXY_PAYLOAD;
    }
    
    for(i=0; i<payloadSize; i++){
      myProtocol._uartTxBuffer.write(rxData[2+i]);
    }
    myProtocol.uartTx();   
    */
//    delay(1);

    
    
//  }//end if SREG_PING != 0
}//end main loop

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




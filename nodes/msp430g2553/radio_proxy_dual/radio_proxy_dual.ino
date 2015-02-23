#include "radio_proxy_dual.h"
#include <SPI.h>
#include <AIR430BoostFCC.h>
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
  pinMode(RADIO_NRF_CE, OUTPUT);
  digitalWrite(RADIO_NRF_CE, LOW);     
  pinMode(RADIO_NRF_CSN, OUTPUT);
  digitalWrite(RADIO_NRF_CSN, HIGH); 
  
  myProtocol.serial_registers[SREG_NODE_ID] = nodeID;
  myProtocol.serial_registers[SREG_TX_PWR] = 0;
  myProtocol.serial_registers[SREG_RSSI] = 0;
  myProtocol.serial_registers[SREG_LQI] = 0;
  myProtocol.serial_registers[SREG_TEMPERATURE] = 0;
  myProtocol.serial_registers[SREG_PING] = 0;
  
  Radio.begin(0x01, CHANNEL_1, POWER_MAX);
#ifdef DUAL_RADIO
 SPI.begin(); 
  nrf24_init();
  nrf24_config(2,PROXY_PAYLOAD);
  nrf24_tx_address(nrf_address);
  nrf24_rx_address(nrf_address);  
#endif
  Serial.begin(9600);
  
  uint8_t temp;
   while(Serial.available()){ //flush rx pipe
      temp = Serial.read();
   }
   

  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, hbt_output);
}



void loop(){
  
  sent = serial_rf_fetch();

  if(myProtocol.serial_registers[SREG_PING] != 0){
    
    //header
    // byte 0 = ID of source node
    // byte 1 = ID of destination node
    txData[0] = nodeID;
    txData[1] = myProtocol.serial_registers[SREG_TARGET];
        
    if( (myProtocol.serial_registers[SREG_PING] & 0x3) == 0x1){
      
      if((myProtocol.serial_registers[SREG_PING] & 0x4) == 0x4){
         //read temperature
         myProtocol.form_packet(&txData[2], READ_REG, SREG_TEMPERATURE, 0);
      }else{
         // read rssi
         myProtocol.form_packet(&txData[2], READ_REG, SREG_RSSI, 0);
      }      
      
      //transmit on 915MHz
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
    }else if( (myProtocol.serial_registers[SREG_PING] & 0x3) == 0x2){
      #ifdef DUAL_RADIO
      SPI.begin();
      //write temperature
      myProtocol.form_packet(&txData[2], WRITE_REG, SREG_TEMPERATURE, myProtocol.serial_registers[SREG_TEMPERATURE]);

      nrf24_send(txData);
      while(nrf24_isSending());
      //TODO something w/ RT count here     
      nrf24_powerUpRx();
      /*
      if(nrf_read_timeout(1000) == 1){//update radio info
        digitalWrite(RED_LED, hbt_output ^= 0x1);
        nrf24_getData(rxData);
        
      }else{//send timeout message
        myProtocol.form_packet(&rxData[2], NACK, 0, TIMEOUT);     
      }*/
      myProtocol.form_packet(&rxData[2], ACK, SREG_TEMPERATURE, myProtocol.serial_registers[SREG_TEMPERATURE]);      

      #else
      myProtocol.form_packet(&rxData[2], NACK, 0, FEATURE_WIP);
      #endif
    }else{ //unknown value in SREG_PING
      myProtocol.form_packet(&rxData[2], NACK, 0, BAD_COMMAND);     
    }    
    myProtocol.serial_registers[SREG_PING] = 0x0; //reset
    
    //send result to Edison
    uint8_t payloadSize = rxData[2+1]+3;
    if(payloadSize > PROXY_PAYLOAD-2){
      payloadSize = PROXY_PAYLOAD;
    }
    for(i=0; i<payloadSize; i++){
      myProtocol._uartTxBuffer.write(rxData[2+i]);
    }
    myProtocol.uartTx();                    
  }//end if SREG_PING != 0
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




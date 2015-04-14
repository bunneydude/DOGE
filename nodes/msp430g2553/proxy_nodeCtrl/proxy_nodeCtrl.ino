#include "proxy_nodeCtrl.h"
#include <SPI.h>
#include <AIR430BoostFCC.h>
#include <nrf24.h>
#include <nRF24L01.h>
#include <Protocol.h>
#include <MyRingBuffer.h>
#include <stdint.h>
#include <cobs.h>

// -----------------------------------------------------------------------------
/**
 *  Global data
 */

// Data to write to radio TX FIFO (60 bytes MAX.)
uint8_t txData[DATA_LENGTH];    

// Data to read from radio RX FIFO (60 bytes MAX.)
uint8_t rxData[DATA_LENGTH];

#define MY_NODE_ID 0x1
#define NODECTRL_VERSION 1

uint8_t nrf_address[5] = {
  0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t nrfRXAttempts = 0;

Protocol myProtocol(0);
uint8_t sent = 0;
uint8_t i = 1;
uint8_t hbt_output = 0x0;
uint8_t sendResponse = 0;
uint8_t e2eSrcID = 0;
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

  myProtocol.serial_registers[SREG_NODE_ID] = MY_NODE_ID;
  myProtocol.serial_registers[SREG_TX_PWR] = 0;
  myProtocol.serial_registers[SREG_RSSI] = 0;
  myProtocol.serial_registers[SREG_LQI] = 0;
  myProtocol.serial_registers[SREG_TEMPERATURE] = 0;
  myProtocol.serial_registers[SREG_PING] = 0;

  Radio.begin(0x01, CHANNEL_1, POWER_MAX);

#ifdef DUAL_RADIO
  SPI.begin(); 
  nrf24_init();
  nrf24_config(2, DATA_LENGTH);
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

  while(!serial_receive(rxData, DATA_LENGTH)); //wait for data


  if(rxData[1] == MY_NODE_ID){
    //try to handle it locally
    if(myProtocol.parse_packet(&rxData[2], &txData[2])){
      txData[0] = MY_NODE_ID;
      txData[1] = rxData[0];
      for(i=0; i<txData[3] + 5; i++){ //size at [3] plus 5 non-payload bytes
        myProtocol._uartTxBuffer.write(txData[i]);
      }
      myProtocol.uartTx();
    }
  }
  else{ //forward it
    e2eSrcID = rxData[0]; //store the original source ID

    Radio.transmit(0x2, rxData, DATA_LENGTH);

    //Make sure radio is ready to receive
    while (Radio.busy());

    // Turn on the receiver and listen for incoming data. Timeout after 1 seconds.
    if (Radio.receiverOn(txData, sizeof(txData), 1000) > 0){
      digitalWrite(RED_LED, hbt_output ^= 0x1);

      //update radio info
      myProtocol.serial_registers[SREG_RSSI] = Radio.getRssi();
      myProtocol.serial_registers[SREG_LQI] = Radio.getLqi();     

    }
    else{ //timeout, send error message
      myProtocol.form_packet(&txData[2], NACK, 0, TIMEOUT);     
      txData[0] = MY_NODE_ID;
      txData[1] = e2eSrcID;
    }
    for(i=0; i<txData[3] + 5; i++){ //size at [3] plus 5 non-payload bytes
      myProtocol._uartTxBuffer.write(txData[i]);
    }
    myProtocol.uartTx();      
  }//end forward  
}//end main loop

// Two streams of data to consider
// This receive code should work no matter which byte is seen first
// If we start in the middle of a frame, finish receiving the frame and discard
//
// When sync'd, first byte should be non-zero and the first of the frame data
// The last byte will be 0
//
// 0, 1, 2, 3, 0, 1, 2, 3, 0, ...
// 0, 1, 0, 1, 0, ...

uint8_t serial_receive(uint8_t* returnBuf, uint8_t size){

  uint8_t buf[size];
  uint8_t index = 0;
  uint8_t rxByte = 1;

  if(Serial.available() >= 2){ //need at least 2 bytes to start receiving
    buf[0] = Serial.read();
    if(buf[0] == 0){ //caught the end of a previous frame
      return 0;
    }
        
    do{ //read until next 0 or we reach max length
      buf[++index] = Serial.read();
    }while((buf[index] != 0) && (index < size-1));
    
    if(buf[index] != 0){ //ran out of space for a frame
      do{
        rxByte = Serial.read();
      }while(rxByte != 0); //get rest of frame out of buffer
      return 0;
    }else{//got a full frame
      index = cobs_decode(buf, index, returnBuf);
      return index; //represents number of bytes decoded
    }
  }
}



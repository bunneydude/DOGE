#include "doge_node_msp_nrf.h"
#include <SPI.h>
#include <Protocol.h>
#include <MyRingBuffer.h>
#include <stdint.h>
#include <nrf24.h>
#include <nRF24L01.h>

//breadboard
///*
#define RADIO1_CE P1_3
#define RADIO1_CSN P1_4
//*/

// Data to write to radio TX FIFO (60 bytes MAX.)
unsigned char txData[7];    

// Data to read from radio RX FIFO (60 bytes MAX.)
unsigned char rxData[7];

uint8_t nodeID = 0x2;
Protocol myProtocol(0);
uint8_t sent = 0;
uint8_t sendResponse = 0;
uint8_t hbt_output = 0x1;
// -----------------------------------------------------------------------------
// Main example

void nrf24_ce_digitalWrite(uint8_t state){
      digitalWrite(RADIO1_CE,state);
}

void nrf24_csn_digitalWrite(uint8_t state){
      digitalWrite(RADIO1_CSN,state);
}

//uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t nrf_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};


uint8_t spi_transfer(uint8_t tx)
{
    uint8_t rx = 0;    
    rx = SPI.transfer(tx);
    return rx;
}

void setup()
{
    pinMode(RADIO1_CE, OUTPUT);
  digitalWrite(RADIO1_CE, LOW);     
  pinMode(RADIO1_CSN, OUTPUT);
  digitalWrite(RADIO1_CSN, HIGH); 
  
  Serial.begin(9600);

  SPI.begin();

  nrf24_init();
  nrf24_config(2,7);
  nrf24_tx_address(nrf_address);
  nrf24_rx_address(nrf_address);  
  
  myProtocol.serial_registers[SREG_NODE_ID] = nodeID;
  myProtocol.serial_registers[SREG_TX_PWR] = 0;
  myProtocol.serial_registers[SREG_RSSI] = 0;
  myProtocol.serial_registers[SREG_LQI] = 0;
  myProtocol.serial_registers[SREG_TEMPERATURE] = 0;
  
  // The radio library uses the SPI library internally, this call initializes
  // SPI/CSn and GDO0 lines. Also setup initial address, channel, and TX power.
//  Radio.begin(0x02, CHANNEL_1, POWER_MAX); 

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
  while(!nrf24_dataReady());
  
  nrf24_getData(rxData); 

    if(rxData[1] == nodeID){
      digitalWrite(RED_LED, hbt_output ^= 0x1);

      //update radio info
      myProtocol.serial_registers[SREG_RSSI] = 0;

      //parse message
      sendResponse = myProtocol.parse_packet(&rxData[2], &txData[2]);        
    
      if(sendResponse == 1){
        txData[0] = nodeID;
        txData[1] = rxData[0]; 
        nrf24_send(txData);
        while(nrf24_isSending());
        nrf24_powerUpRx();
      }
 
      //Update settings
      //TODO, channel, power, nodeID
    }   
}

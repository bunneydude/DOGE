#include <stdint.h>
#include "proxy_nodeCtrl.h"
#include <SPI.h>
#include <AIR430BoostFCC.h>
//#include <nrf24.h>
//#include <nRF24L01.h>
#include <protocol.h>
/*#include <MyRingBuffer.h>*/
#include <cobs.h>
#include <platform.h>

// -----------------------------------------------------------------------------
/**
 *  Global data
 */

dogePacket txPacket;
appPacket* txAppPacket;
packetAttr txAttr;

dogePacket rxPacket;
appPacket* rxAppPacket;
packetAttr rxAttr;

#define static_assert_root(cond) uint8_t static_assert_root[((cond) == 1) ? 1 : -1]
static_assert_root(MY_NODE_ID == ROOT_NODE);

uint8_t serialBuffer[MAX_DATA_LENGTH + 2]; //+1 for COBS overhead, another +1 for framing byte

uint8_t nrf_address[5] = {
  0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t nrfRXAttempts = 0;

struct Protocol myProtocol;
uint8_t sent = 0;
uint8_t i = 1;
uint8_t hbt_output = 0x1;
uint8_t sendResponse = 0;
uint16_t e2eSrcID = 0;
// -----------------------------------------------------------------------------
//functions for nRF
/*
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
*/
// Main example

void setup()
{
  setup_timer_hw();
  pinMode(RADIO_NRF_CE, OUTPUT);
  digitalWrite(RADIO_NRF_CE, LOW);     
  pinMode(RADIO_NRF_CSN, OUTPUT);
  digitalWrite(RADIO_NRF_CSN, HIGH); 

  Radio.begin(MY_NODE_ID, CHANNEL_1, POWER_MAX);

/*
#ifdef DUAL_RADIO
  SPI.begin(); 
  nrf24_init();
  nrf24_config(2, DATA_LENGTH);
  nrf24_tx_address(nrf_address);
  nrf24_rx_address(nrf_address);  
#endif
*/
  Serial.begin(9600);

  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, hbt_output);

  memset(&txPacket, 0, sizeof(dogePacket));
  memset(&rxPacket, 0, sizeof(dogePacket));  
  txAppPacket = (appPacket*)((uint8_t*)&txPacket + RAW_PACKET_DATA_OFFSET);
  rxAppPacket = (appPacket*)((uint8_t*)&rxPacket + RAW_PACKET_DATA_OFFSET);
}

void loop(){

  while(serial_receive((uint8_t*)(&rxPacket)) == 0); //wait for data

  if(rxPacket.hdr.dst == MY_NODE_ID){ //ACK back same address and data
    application_form_packet(txAppPacket, &txAttr, CMD_ACK, rxAppPacket->addr, rxAppPacket->data);
    link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, MY_NODE_ID, rxPacket.hdr.src, MY_NODE_ID, rxPacket.hdr.shSrc);  
  }
  else{ //forward it
    copy_doge_packet(&txPacket, &rxPacket);
    reliable_transmit();

    //Make sure radio is ready to receive
    while (Radio.busy());

    // Turn on the receiver and listen for incoming data. Timeout after 1 seconds.
    if (reliable_receive(TIMEOUT_1000_MS))
    {
      copy_doge_packet(&txPacket, &rxPacket);
      digitalWrite(RED_LED, hbt_output ^= 0x1);
    }
    else{ //timeout, send error message
      application_form_packet(txAppPacket, &txAttr, CMD_NACK, rxAppPacket->addr, ERR_TIMEOUT);
      link_layer_form_packet(&txPacket, &txAttr, RAW_PACKET, MY_NODE_ID, rxPacket.hdr.src, MY_NODE_ID, rxPacket.hdr.shSrc); 
    }
  }//end forward
  serial_transmit((uint8_t*)(&txPacket), RAW_PACKET_TOTAL_SIZE(&txPacket), 1);
}//end main loop




// This receive code should work no matter which byte is seen first
// If we start in the middle of a frame, finish receiving the frame and discard
//
// When sync'd, first byte should be non-zero and the first of the frame data
// The last byte will be 0 or else it means we ran out of space (indicates error on sender-side)

uint8_t serial_receive(uint8_t* returnBuf){

  uint8_t index = 0;
  uint8_t rxByte = 1;

  if(Serial.available() >= 2){ //need at least 2 bytes to start receiving
    serialBuffer[0] = Serial.read();
    if(serialBuffer[0] == 0){ //caught the end of a previous frame
      return 0;
    }

    do{ //read until next 0 or we reach max length
      while(!Serial.available());
      serialBuffer[++index] = Serial.read();
    }
    while((serialBuffer[index] != 0) && (index < (MAX_DATA_LENGTH+2)-1)); 
    //MAX_DATA_LENGTH+2 is the size of serialBuffer; extra -1 is to account for 0-based indexing

    if(serialBuffer[index] != 0){ //ran out of space for a frame
      do{
        while(!Serial.available());
        rxByte = Serial.read();
      }
      while(rxByte != 0); //get rest of frame out of uart buffer
      return 0; //discard received bytes

    }
    else{//got a full frame
      index = cobs_decode(serialBuffer, index, returnBuf);
      return index; //represents number of bytes decoded
    }
  }
  else{
    return 0; //wait for more bytes
  }
}

//encode size bytes from buf with COBS
uint8_t serial_transmit(uint8_t* buf, uint8_t size, uint8_t encode){
  uint8_t rc = 0;
  uint8_t i = 0;

  if(encode == 1){
    rc = cobs_encode(buf, size, serialBuffer);
    for(i=0; i<rc; i++){
      Serial.write(serialBuffer[i]);
    }
  }
  else{
    for(i=0; i<size; i++){
      Serial.write(buf[i]);
    }
  }

  Serial.write((uint8_t)0); //indicate end of frame
}

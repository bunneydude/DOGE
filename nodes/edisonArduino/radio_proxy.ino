
#include "radio_proxy.h"
#include <Protocol.h>
#include <MyRingBuffer.h>
#include <IPCBuffer.h>
#include <stdint.h>

// state
uint8_t hbt_output = 0;
uint8_t temp=0;

uint8_t data = 0;

uint8_t state = 0;
Protocol myProtocol(1);

IPCBuffer proxyCmdBuffer(3);
IPCBuffer proxyRxBuffer(4);

unsigned long firstStamp;   
unsigned long lastStamp;
uint8_t printTime = 0x0;

void setup() {                

  //   system("telnetd -l /bin/sh"); //set up Telnet
  //   system("ifconfig eth0 169.254.1.1 netmask 255.255.0.0 up");

  //initialize pins
  pinMode(HBT_LED, OUTPUT);   
  Serial.begin(SERIAL_BAUD);
  Serial1.begin(9600);

  while(Serial1.available()){ //flush
    temp = Serial1.read();
  }

  digitalWrite(HBT_LED, 0x1);
  hbt_output = 0x1;

  proxyCmdBuffer.open(CMD_BUFFER_LENGTH, CMD_BUFFER_WIDTH);
  proxyRxBuffer.open(RX_BUFFER_LENGTH, RX_BUFFER_WIDTH);

  delay(250);
  digitalWrite(HBT_LED, hbt_output^=0x01);  
  delay(250);
  digitalWrite(HBT_LED, hbt_output^=0x01);  
  delay(250);
  digitalWrite(HBT_LED, hbt_output^=0x01);  
  delay(250);
}


void loop() {

  digitalWrite(HBT_LED, hbt_output^=0x01);  
  firstStamp = micros();   
  //   sketch_respond(sketchCmdBuffer, sketchRxBuffer);
  proxy_respond(proxyCmdBuffer, proxyRxBuffer); 
  lastStamp = micros();

  if(printTime == 0x1){
    Serial.println("Time stamp uartTx:");
    Serial.println(lastStamp - firstStamp); 
    Serial.println(firstStamp);
    Serial.println(lastStamp);
    Serial.println(" ");
    printTime = 0x0;
  }

}

void proxy_respond(IPCBuffer buf, IPCBuffer resp){
  printTime = 0x1;
  uint8_t rxBuffer[7];
  uint8_t response[7];   
  uint8_t index = 0;
  uint8_t i = 0;

  //get message from cmd buffer
  buf.read(&rxBuffer[index++]); //srcID
  buf.read(&rxBuffer[index++]); //dstID
  buf.read(&rxBuffer[index++]); //cmd
  buf.read(&rxBuffer[index++]); //size

  for(i=0; i<rxBuffer[3]; i++){ //get rest of bytes
    buf.read(&rxBuffer[index++]);
  }

  buf.read(&rxBuffer[index++]); //get checksum


#ifdef PROXY_MSP
  //send command to proxy mcu
  for(i=0; i<rxBuffer[3]+5; i++){
    myProtocol._uartTxBuffer.write(rxBuffer[i]);
  }

  myProtocol.uartTx();

  //get response from proxy mcu
  //start rx
  index = 0;
  while(Serial1.available() < 4); //wait for 4 bytes

  //first bytes rx      
  response[index++] = Serial1.read(); //srcID
  response[index++] = Serial1.read(); //dstID 
  response[index++] = Serial1.read(); //cmd
  response[index++] = Serial1.read(); //size     

  //mid rx
  for(i=0; i<response[3]; i++){ //get rest of bytes
    while(Serial1.available() == 0); //wait for a byte
    response[index++] = Serial1.read();
  }

  while(Serial1.available() == 0); //wait for checksum
  response[index++] = Serial1.read();
  //end rx
#else
  //simulate data
  myProtocol.form_packet(&response[2], ACK, 0, FEATURE_WIP);
  response[0] = 2;
  response[1] = 1;
#endif

  //write response to ipc buffer
  for(i=0; i<response[3] + 5; i++){
    resp.write(&response[i]);
  }
}



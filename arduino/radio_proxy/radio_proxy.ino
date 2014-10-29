
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

//TTYUARTClass test1 = Serial1;

//IPCBuffer sketchCmdBuffer(1);
//IPCBuffer sketchRxBuffer(2);
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

//   sketchCmdBuffer.open(CMD_BUFFER_LENGTH, CMD_BUFFER_WIDTH);
//   sketchRxBuffer.open(RX_BUFFER_LENGTH, RX_BUFFER_WIDTH);

   proxyCmdBuffer.open(CMD_BUFFER_LENGTH, CMD_BUFFER_WIDTH);
   proxyRxBuffer.open(RX_BUFFER_LENGTH, RX_BUFFER_WIDTH);
   
   delay(2000);
}


void loop() {

   //  if(1 == task_hbt){
   //    task_hbt = TASK_HBT_PERIOD;

   //  }    

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

//delay(1000); 
}

void sketch_respond(IPCBuffer buf, IPCBuffer resp){
   if(buf.available()){
      printTime = 0x1;
      uint8_t rxBuffer[5];
      uint8_t response[5];   
      uint8_t index = 0;
      uint8_t i = 0;
      uint8_t sendResponse = 0;

//get packet from ipc buffer
      while(buf.available() < 2); //wait for 2 bytes
      buf.read(&rxBuffer[index++]); //cmd
      buf.read(&rxBuffer[index++]); //size

      for(i=0; i<rxBuffer[1]; i++){ //get rest of bytes
         while(buf.available() == 0); //wait for a byte
         buf.read(&rxBuffer[index++]);
      }

      while(buf.available() == 0); //wait for checksum
      buf.read(&rxBuffer[index++]);

//parse packet
      sendResponse = myProtocol.parse_packet(rxBuffer, response);

//send response to ipc buffer
      if(sendResponse == 1){
         for(i=0; i<response[1]+3; i++){
            resp.write(&response[i]);
         }
      }
   } 
}

void proxy_respond(IPCBuffer buf, IPCBuffer resp){
   if(buf.available()){
      printTime = 0x1;
      uint8_t rxBuffer[5];
      uint8_t response[5];   
      uint8_t index = 0;
      uint8_t i = 0;

//get message from cmd buffer
      while(buf.available() < 2); //wait for 2 bytes
      buf.read(&rxBuffer[index++]); //cmd
      buf.read(&rxBuffer[index++]); //size

      for(i=0; i<rxBuffer[1]; i++){ //get rest of bytes
         while(buf.available() == 0); //wait for a byte
         buf.read(&rxBuffer[index++]);
      }

      while(buf.available() == 0); //wait for checksum
      buf.read(&rxBuffer[index++]);

//send command to proxy mcu
      for(i=0; i<rxBuffer[1]+3; i++){
         myProtocol._uartTxBuffer.write(rxBuffer[i]);
      }

      myProtocol.uartTx();

//get response from proxy mcu
//start rx
      index = 0;
      while(Serial1.available() < 2); //wait for 2 bytes
      
//first bytes rx      
      response[index++] = Serial1.read(); //cmd
      response[index++] = Serial1.read(); //size     

//mid rx
      for(i=0; i<response[1]; i++){ //get rest of bytes
         while(Serial1.available() == 0); //wait for a byte
         response[index++] = Serial1.read();
      }

      while(Serial1.available() == 0); //wait for checksum
      response[index++] = Serial1.read();
//end rx


//pre tx - start rx = 441us
//start rx - first bytes rx = 14220
//mid rx - end rx = 20us

//write response to ipc buffer
      for(i=0; i<response[1]+3; i++){
         resp.write(&response[i]);
      }
   } 
}

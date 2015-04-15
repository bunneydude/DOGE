#include <IPCBuffer.h>
#include <stdint.h>
#include "radio_proxy.h"

// state
uint8_t hbt_output = 0;

IPCBuffer fromPythonBuffer(3);
IPCBuffer toPythonBuffer(4);
uint8_t serialBuffer[MAX_DATA_LENGTH + 2]; 

uint8_t bytesRead = 0;
uint8_t i = 0;

void setup() {                

   //initialize pins
   pinMode(HBT_LED, OUTPUT);   
   Serial.begin(SERIAL_BAUD); //for debug
   Serial1.begin(9600); //to MSP430

   digitalWrite(HBT_LED, 0x1);
   hbt_output = 0x1;

   fromPythonBuffer.open(CMD_BUFFER_LENGTH, CMD_BUFFER_WIDTH);
   toPythonBuffer.open(RX_BUFFER_LENGTH, RX_BUFFER_WIDTH);

   delay(250);
   digitalWrite(HBT_LED, hbt_output^=0x01);  
   delay(250);
   digitalWrite(HBT_LED, hbt_output^=0x01);  
   delay(250);
   digitalWrite(HBT_LED, hbt_output^=0x01);  
   delay(250);
}

void loop() {

   if(Serial1.available() > 0){ //read from Serial1, send to python
      digitalWrite(HBT_LED, hbt_output^=0x01);  
      
      bytesRead = 0;
      do{
         serialBuffer[bytesRead++] = Serial1.read();
      }while(serialBuffer[bytesRead-1] != 0);

      for(i=0; i<bytesRead; i++){
         toPythonBuffer.write(&serialBuffer[i]);
      }      
   }

   if(fromPythonBuffer.available() > 0){ //read from python, send to Serial1
      digitalWrite(HBT_LED, hbt_output^=0x01);  

      bytesRead = 0;
      do{
         fromPythonBuffer.read(&serialBuffer[bytesRead++]);
      }while(serialBuffer[bytesRead-1] != 0);

      for(i=0; i<bytesRead; i++){
         Serial1.write(serialBuffer[i]);
      }  
   }

   delay(1); //1ms delay since there aren't interrupts on these buffers
}

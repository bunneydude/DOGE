#include <stdint.h>
#include <stdio.h>
#include "../../cobs/cobs.h"
#include "test.h"

uint8_t inputBuf[] = {1,2,3};
uint8_t encBuf[] = {0,0,0,0};
uint8_t decBuf[] = {0,0,0};
uint8_t rc = 0;

uint8_t counter = 0;


void main(){
   test_first_0();
   test_long_frame(); 
   test_many_0();
   test_single_char();
}


uint8_t stream_read(uint8_t* stream){
   return stream[counter++];
}

uint8_t serial_receive(uint8_t* returnBuf, uint8_t size, uint8_t* stream, uint8_t streamSize){

  uint8_t buf[size];
  uint8_t index = 0;
  uint8_t rxByte = 1;

  if((streamSize - counter) >= 2){ //need at least 2 bytes to start receiving
    buf[0] = stream_read(stream);
    if(buf[0] == 0){ //caught the end of a previous frame
      printf("   Buf[0] was 0\n");
      return 0;
    }
   printf("   Buf[0] = %d\n", buf[0]);
        
    do{ //read until next 0 or we reach max length
      buf[++index] = stream_read(stream);
      printf("   Read %d\n", buf[index]);
    }while((buf[index] != 0) && (index < size-1));
    
    if(buf[index] != 0){ //ran out of space for a frame
      do{
        rxByte = stream_read(stream);
      }while(rxByte != 0); //get rest of frame out of buffer
      printf("   Ran out of space\n");
      return 0;
    }else{//got a full frame
      printf("   Got full frame. Index = %d\n", index);
      index = cobs_decode(buf, index, returnBuf);
      printf("   Bytes unstuffed = %d\n", index);
      return index; //represents number of bytes decoded
    }
  }
   printf("Nothing to read\n");
}

uint8_t test_first_0(){

   uint8_t stream[] = {0, 4, 1, 2, 3, 0, 2, 5, 0};
   counter = 0;
   uint8_t response[5];
   uint8_t rc, i;

   printf("Test: first char 0\n");

   rc = serial_receive(response, 5, stream, sizeof(stream));
   printf("   rc = %d\n", rc);
   for(i=0; i<5; i++) printf("  %02X",response[i]); printf("\n");


   rc = serial_receive(response, 5, stream, sizeof(stream));
   printf("   rc = %d\n", rc);
   for(i=0; i<5; i++) printf("  %02X",response[i]); printf("\n");


   rc = serial_receive(response, 5, stream, sizeof(stream));
   printf("   rc = %d\n", rc);
   for(i=0; i<5; i++) printf("  %02X",response[i]); printf("\n\n");
   return 1;
}

uint8_t test_long_frame(){

   uint8_t stream[] = {4, 1, 2, 3, 0, 6, 1, 2, 3, 4, 5, 0};
   counter = 0;
   uint8_t rc, i;
   uint8_t response[6];

   rc = serial_receive(response, 5, stream, sizeof(stream));
   printf("Test: long frame\n");
   printf("   rc = %d\n", rc);
   for(i=0; i<6; i++) printf("  %02X",response[i]); printf("\n");

   rc = serial_receive(response, 5, stream, sizeof(stream));
   printf("   rc = %d\n", rc);
   for(i=0; i<6; i++) printf("  %02X",response[i]); printf("\n\n");
}


uint8_t test_many_0(){
   uint8_t stream[] = {0, 0, 0, 4, 1, 2, 3, 0, 0, 2, 6, 0};
   counter = 0;
   uint8_t rc, i;
   uint8_t response[6] = {0, 0, 0, 0, 0, 0};

   printf("Test: many 0s\n");

   rc = serial_receive(response, 6, stream, sizeof(stream));
   printf("   rc = %d\n", rc);
   for(i=0; i<rc; i++) printf("  %02X",response[i]); printf("\n");

   rc = serial_receive(response, 6, stream, sizeof(stream));
   printf("   rc = %d\n", rc);
   for(i=0; i<rc; i++) printf("  %02X",response[i]); printf("\n");

   rc = serial_receive(response, 6, stream, sizeof(stream));
   printf("   rc = %d\n", rc);
   for(i=0; i<rc; i++) printf("  %02X",response[i]); printf("\n");

   rc = serial_receive(response, 6, stream, sizeof(stream));
   printf("   rc = %d\n", rc);
   for(i=0; i<rc; i++) printf("  %02X",response[i]); printf("\n");

   rc = serial_receive(response, 6, stream, sizeof(stream));
   printf("   rc = %d\n", rc);
   for(i=0; i<rc; i++) printf("  %02X",response[i]); printf("\n");

   rc = serial_receive(response, 6, stream, sizeof(stream));
   printf("   rc = %d\n", rc);
   for(i=0; i<rc; i++) printf("  %02X",response[i]); printf("\n\n");
}


uint8_t test_single_char(){
   uint8_t stream[] = {1, 1, 0, 2, 1, 0};
   counter = 0;
   uint8_t rc, i;
   uint8_t response[6] = {0, 0, 0, 0, 0, 0};

   printf("Test: single char\n");

   rc = serial_receive(response, 6, stream, sizeof(stream));
   printf("   rc = %d\n", rc);
   for(i=0; i<rc; i++) printf("  %02X",response[i]); printf("\n");
   
   rc = serial_receive(response, 6, stream, sizeof(stream));
   printf("   rc = %d\n", rc);
   for(i=0; i<rc; i++) printf("  %02X",response[i]); printf("\n");
}

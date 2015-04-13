#include <stdint.h>
#include <stdio.h>
#include "../../cobs/cobs.h"


uint8_t inputBuf[] = {1,2,3};
uint8_t encBuf[] = {0,0,0,0};
uint8_t decBuf[] = {0,0,0};
uint8_t rc = 0;

void main(){
   int i;

   printf("Try to encode buffer of length %d.\n", sizeof(inputBuf));
   for(i=0; i<sizeof(inputBuf); i++) printf(" %02X", inputBuf[i]); printf("\n");

   rc = cobs_encode(inputBuf, sizeof(inputBuf), encBuf);

   for(i=0; i<rc; i++) printf(" %02X", encBuf[i]); printf("\n");
   
}

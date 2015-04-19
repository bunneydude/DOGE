#include <stdint.h>
#include <string.h>
#include "../packet.h"
#include "protocol.h"

void Protocol_init(struct Protocol* obj){
   memset(obj, 0, sizeof(struct Protocol));
   obj->dataRegisters = ((&(memoryMap[MM_PHYSICAL_BAR/4].u8[MM_PHYSICAL_BAR%4])));
}


uint8_t Protocol_parse_packet(struct Protocol* obj, uint8_t* buf, uint8_t* response){
   uint8_t returnCode = 0;
   uint8_t returnData = 0;

   obj->cmd = buf[0];
   obj->size = buf[1];
   obj->checksum = obj->cmd + obj->size;
   uint8_t i=0;
   for(i=0; i<obj->size; i++){
      obj->payload[i] = buf[2+i];
      obj->checksum += obj->payload[i];
   }
   obj->checksum += buf[2+obj->size]; //get last byte, the checksum

   if(obj->checksum == 0){//checksum matches
      obj->executePacket = 1;
   }else{ //bad checksum
      Protocol_form_packet(response, CMD_NACK, 0, ERR_CHECKSUM);
      returnCode = 1;
      obj->executePacket = 0;
      obj->byteNumber = 0;
      obj->checksum = 0;
   }

   if(obj->executePacket == 1){
      obj->executePacket = 0;
      obj->checksum = 0;
      switch(obj->cmd){
         case(CMD_READ_REG): //read_reg
            obj->addr = obj->payload[0];

            if(obj->addr >= MM_FUNCTION_MAX){
               Protocol_form_packet(response, CMD_NACK, 0, ERR_RANGE);
            }else{
               if(obj->addr < MM_PHYSICAL_MAX){
                  returnData = obj->dataRegisters[obj->addr];
               }else{
                  check_mm_space(1, obj->addr, &returnData, 0);
               }
               Protocol_form_packet(response, CMD_ACK, obj->addr, returnData);
            }
            returnCode = 1;
            break;

         case(CMD_WRITE_REG):
            //Serial.println("Got write_reg");
            obj->addr = obj->payload[0];
            obj->data = obj->payload[1];

            if(obj->addr >= MM_FUNCTION_MAX){
               Protocol_form_packet(response, CMD_NACK, 0, ERR_RANGE);
            }else{
               if(obj->addr < MM_PHYSICAL_MAX){
                  obj->dataRegisters[obj->addr] = obj->data;
               }else{
                  check_mm_space(0, obj->addr, &(obj->data), 0);
               }
               Protocol_form_packet(response, CMD_ACK, obj->addr, obj->data);
            }
            returnCode = 1;
            break;

         case(CMD_ACK): //ack
            //Serial.println("Got ack");
            //Serial.print("Addr = ");Serial.println(_remainderBuf[0]);
            //Serial.print("Data = ");Serial.println(_remainderBuf[1]);
            //Serial.print("\n");
            returnCode = 0;
            break;

         case(CMD_NACK): //nack
            //Serial.println("Got nack");
            //Serial.print("Addr = ");Serial.println(_remainderBuf[0]);
            //Serial.print("Data = ");Serial.println(_remainderBuf[1]);
            //Serial.print("\n");
            returnCode = 0;
            break;

         case(CMD_NOP): //nop
            //Serial.println("Got nop");
            returnCode = 0;
            break;

         default: //unknown command
            //Serial.println("Got unknown command. Send nack");
            Protocol_form_packet(response, CMD_NACK, 0, ERR_COMMAND);
            returnCode = 1;
            break;
      }//end switch
   }//end if executePacket==1
   return returnCode;
}//end parse_packet

uint8_t Protocol_form_packet(uint8_t* buf, uint8_t cmd, uint8_t addr, uint8_t data){
   uint8_t index = 0;
   uint8_t checksum = cmd;

   buf[index++] = cmd;

   if(cmd == CMD_READ_REG){
      buf[index++] = (1);
      buf[index++] = (addr);

      checksum += 1;
      checksum += addr;

   }else if(cmd == CMD_WRITE_REG){
      buf[index++] = (2);
      buf[index++] = (addr);
      buf[index++] = (data);

      checksum += 2;
      checksum += addr;
      checksum += data;

   }else if(cmd == CMD_ACK){
      buf[index++] = (2);
      buf[index++] = (addr);
      buf[index++] = (data);

      checksum += 2;
      checksum += addr;
      checksum += data;

   }else if(cmd == CMD_NACK){
      buf[index++] = (2);
      buf[index++] = (addr);
      buf[index++] = (data);

      checksum += 2;
      checksum += addr;
      checksum += data;

   }else if(cmd == CMD_NOP){
      buf[index++] = ((uint8_t)0);

   }else{//unknown command
      return 0;
   }

   buf[index++] = ((~checksum)+1);

   return 1;
}

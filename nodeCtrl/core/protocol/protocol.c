#include <stdint.h>
#include <string.h>
#include <packet.h>
#include "protocol.h"

void Protocol_init(struct Protocol* obj){
   memset(obj, 0, sizeof(struct Protocol));
   obj->dataRegisters = ((&(memoryMap[MM_PHYSICAL_BAR/4].u8[MM_PHYSICAL_BAR%4])));
}

int link_layer_parse_packet(struct Protocol* obj, rawPacket* message, rawPacket* response)
{
   uint8_t typeAck;
   packetType type;
   uint8_t packetId;
   uint8_t rta;
   uint8_t status;

   GET_HEADER_TYPE_ACK(message->hdr.type, typeAck);
   GET_HEADER_TYPE(message->hdr.type, type);
   GET_TXINFO_PACKET_ID(message->hdr.txInfo, packetId);
   GET_TXINFO_RTA(message->hdr.txInfo, rta);
   // Check CRC before parsing the data payload
   // TODO: Replace this CRC check later on with a generic packet CRC check
   status = check_raw_packet_crc(message);
   if (status == ERR_CHECKSUM){
      //TODO: mess up packet
      return status;
   }
   if (message->hdr.type == RAW_PACKET){
      application_parse_packet(obj,
                               (appPacket*)((void*)message + RAW_PACKET_DATA_OFFSET), 
                               (appPacket*)((void*)response + RAW_PACKET_DATA_OFFSET));

   }
}

uint8_t application_parse_packet(struct Protocol* obj, appPacket* message, appPacket* response){
   uint8_t returnCode = 0;
   uint8_t returnData = 0;

   switch(message->cmd){
      case(CMD_READ_REG): //read_reg
         if(message->addr >= MM_FUNCTION_MAX){
            application_form_packet(response, CMD_NACK, 0, ERR_RANGE);
         }else{
            if(message->addr < MM_PHYSICAL_MAX){
               returnData = obj->dataRegisters[message->addr];
            }else{
               check_mm_space(1, message->addr, &returnData, 0);
            }
            application_form_packet(response, CMD_ACK, message->addr, returnData);
         }
         returnCode = 1;
         break;

      case(CMD_WRITE_REG):
         //Serial.println("Got write_reg");

         if(message->addr >= MM_FUNCTION_MAX){
            application_form_packet(response, CMD_NACK, 0, ERR_RANGE);
         }else{
            if(message->addr < MM_PHYSICAL_MAX){
               obj->dataRegisters[message->addr] = message->data;
            }else{
               check_mm_space(0, message->addr, &message->data, 0);
            }
            application_form_packet(response, CMD_ACK, message->addr, message->data);
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
         application_form_packet(response, CMD_NACK, 0, ERR_COMMAND);
         returnCode = 1;
         break;
   }//end switch
return returnCode;
}//end application_parse_packet

uint8_t application_form_packet(appPacket* response, uint8_t cmd, uint8_t addr, uint8_t data){
   response->cmd = cmd;
   switch(response->cmd){
      case(CMD_READ_REG):
         response->addr = (addr);
         break;
      case(CMD_WRITE_REG):
         response->addr = (addr);
         response->data = (data);
         break;
      case(CMD_ACK):
         response->addr = (addr);
         response->data = (data);
      case(CMD_NACK):
         response->addr = (addr);
         response->data = (data);
         break;
      case(CMD_NOP):
         response->size = ((uint8_t)0);
      default: //unknown command
         return 0;
   }
   return 1;
}

#include <stdint.h>
#include <string.h>
#include "packet.h"
#include "protocol.h"

void Protocol_init(struct Protocol* obj){
   memset(obj, 0, sizeof(struct Protocol));
   obj->dataRegisters = ((&(memoryMap[MM_PHYSICAL_BAR/4].u8[MM_PHYSICAL_BAR%4])));
}

uint8_t link_layer_parse_packet(struct Protocol* obj, rawPacket* message, rawPacket* response)
{
   uint8_t typeAck         = 0;
   packetType type         = (packetType)0;
   uint8_t packetId        = 0;
   uint8_t rta             = 0;
   packetAttr messageAttr  = {0};
   packetAttr responseAttr = {0};
   uint8_t status          = 0;
   uint8_t sendResponse    = NO_TRANSMIT;

   // Decode header info
   GET_HEADER_TYPE_ACK(message->hdr.type, typeAck);
   GET_HEADER_TYPE(message->hdr.type, type);
//   type = (packetType) ((message->hdr.type & HEADER_TYPE_MASK) >> HEADER_TYPE_SHIFT);
   GET_TXINFO_PACKET_ID(message->hdr.txInfo, packetId);
   GET_TXINFO_RTA(message->hdr.txInfo, rta);

   // Clear response data
   memset(response, 0, sizeof(rawPacket));
   // Check message CRC before parsing the data payload
   // TODO: Replace this CRC check later on with a generic packet CRC check

   status = check_raw_packet_crc(message);
   if (status == ERR_CHECKSUM){
      //TODO: log error and/or send error message back?
      sendResponse = NO_TRANSMIT;

   }else if (message->hdr.type == RAW_PACKET){
      messageAttr.ack = typeAck;
      messageAttr.size = message->size;
      status = application_parse_packet(obj,
                                        (appPacket*)((void*)message + RAW_PACKET_DATA_OFFSET),
                                        (appPacket*)((void*)response + RAW_PACKET_DATA_OFFSET),
                                        &messageAttr,
                                        &responseAttr);      
      if (responseAttr.ack == TRUE){ // Application layer requests a response packet
         link_layer_form_packet(response, &responseAttr, RAW_PACKET, message->hdr.dst, message->hdr.src);
         sendResponse = TRANSMIT_RESPONSE;
      }
   }
   return sendResponse;
}

/**
 *message = source/destination.
 */
uint8_t link_layer_form_packet(rawPacket* packet, packetAttr* attr, uint8_t type, uint16_t src, uint16_t dst)
{
   memset(packet, 0, sizeof(packet->hdr));
   SET_HEADER_TYPE(packet->hdr.type, type);
   SET_HEADER_TYPE_ACK(packet->hdr.type, attr->ack);
   packet->hdr.src = src;
   packet->hdr.dst = dst;
   //TODO: Packet ID/RTA/CRC are supposed to be set by MAC protocol
   /*SET_TXINFO_PACKET_ID(response->hdr.txInfo, 0);*/
   /*SET_TXINFO_RTA(response->hdr.txInfo, 0);*/
   packet->hdr.ttl = DEFAULT_PACKET_TTL;
   packet->size = attr->size;
   add_raw_packet_crc(packet);
   return 0;
}

uint8_t application_parse_packet(struct Protocol* obj, appPacket* message, appPacket* response, packetAttr* messageAttr, packetAttr* responseAttr)
{
   uint8_t returnCode = 0;
   uint8_t returnData = 0;

   switch(message->cmd){
      case(CMD_READ_REG): //read_reg
         if(message->addr >= MM_FUNCTION_MAX){
            application_form_packet(response, responseAttr, CMD_NACK, 0, ERR_RANGE);
         }else{
            if(message->addr < MM_PHYSICAL_MAX){
               returnData = obj->dataRegisters[message->addr];
            }else{
               check_mm_space(1, message->addr, &returnData, 0);
            }
            application_form_packet(response, responseAttr, CMD_ACK, message->addr, returnData);
         }
         returnCode = 1;
         break;

      case(CMD_WRITE_REG):
         //Serial.println("Got write_reg");

         if(message->addr >= MM_FUNCTION_MAX){
            application_form_packet(response, responseAttr, CMD_NACK, 0, ERR_RANGE);
         }else{
            if(message->addr < MM_PHYSICAL_MAX){
               obj->dataRegisters[message->addr] = message->data;
            }else{
               check_mm_space(0, message->addr, &message->data, 0);
            }
            application_form_packet(response, responseAttr, CMD_ACK, message->addr, message->data);
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
         application_form_packet(response, responseAttr, CMD_NACK, 0, ERR_COMMAND);
         returnCode = 1;
         break;
   }//end switch
   return returnCode;
}//end application_parse_packet

uint8_t application_form_packet(appPacket* packet, packetAttr* attr, uint8_t cmd, uint8_t addr, uint8_t data){
   packet->cmd = cmd;
   switch(packet->cmd){
      case(CMD_READ_REG):
         packet->addr = (addr);
         attr->size = CMD_READ_REG_DATA_SIZE;
         break;
      case(CMD_WRITE_REG):
         packet->addr = (addr);
         packet->data = (data);
         attr->size = CMD_WRITE_REG_DATA_SIZE;
         break;
      case(CMD_ACK):
         packet->addr = (addr);
         packet->data = (data);
         attr->ack = TRUE;
         attr->size = CMD_ACK_DATA_SIZE;
         break;
      case(CMD_NACK):
         packet->addr = (addr);
         packet->data = (data);
         attr->ack = TRUE;
         attr->size = CMD_NACK_DATA_SIZE;
         break;
      case(CMD_NOP):
         attr->size = CMD_NOP_DATA_SIZE;
         break;
      default: //unknown command
         return 0;
   }
   return 1;
}

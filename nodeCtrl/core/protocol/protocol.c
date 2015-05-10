#include <stdint.h>
#include <string.h>
#include "../packet.h"
#include "protocol.h"

void Protocol_init(struct Protocol* obj){
   memset(obj, 0, sizeof(struct Protocol));
   obj->dataRegisters = ((&(memoryMap[MM_PHYSICAL_BAR/4].u8[MM_PHYSICAL_BAR%4])));
}

/**
 * @brief Parses link layer packets and passes the data payload to application layer.
 */
uint8_t link_layer_parse_packet(struct Protocol* obj, dogePacket* message, dogePacket* response)
{
   uint8_t ack             = 0;
   packetType type         = (packetType)0;
   uint8_t packetId        = 0;
   uint8_t rta             = 0;
   packetAttr messageAttr  = {0};
   packetAttr responseAttr = {0};
   uint8_t status          = 0;
   uint8_t sendResponse    = NO_TRANSMIT;

   // Decode header info
   ack      = GET_HEADER_TYPE_ACK(message->hdr.type);
   type     = GET_HEADER_TYPE(message->hdr.type);
   packetId = GET_TXINFO_PACKET_ID(message->hdr.txInfo);
   rta      = GET_TXINFO_RTA(message->hdr.txInfo);

   // Clear response data
   memset(response, 0, sizeof(dogePacket));
   // Check message CRC before parsing the data payload
   status = check_packet_crc(message);
   if (status == ERR_CHECKSUM){
      //TODO: log error and/or send error message back?
      sendResponse = NO_TRANSMIT;
   }else if (type == RAW_PACKET){
      messageAttr.ack = ack;
      messageAttr.size = RAW_PACKET_DATA_SIZE(message);
      status = application_parse_packet(obj,
                                        (appPacket*)((uint8_t*)message + RAW_PACKET_DATA_OFFSET),
                                        (appPacket*)((uint8_t*)response + RAW_PACKET_DATA_OFFSET),
                                        &messageAttr,
                                        &responseAttr);
      if (responseAttr.ack == TRUE){ // Application layer requests a response packet
         link_layer_form_packet(response, &responseAttr, RAW_PACKET, message->hdr.dst, message->hdr.src, message->hdr.shDst, message->hdr.shSrc);
         sendResponse = TRANSMIT_RESPONSE;
      }
   }
   else if (type == SIGNALING_BROADCAST_BEACON){
      responseAttr.ack = FALSE;
      responseAttr.size = 0; //No data payload
      link_layer_form_packet(response, &responseAttr, SIGNALING_UNICAST_BEACON, message->hdr.dst, message->hdr.src, message->hdr.shDst, message->hdr.shSrc);
      sendResponse = TRANSMIT_RESPONSE;
   }
   else if (type == SIGNALING_UNICAST_BEACON){
      responseAttr.ack = TRUE;
      responseAttr.size = 0; //No data payload
      link_layer_form_packet(response, &responseAttr, SIGNALING_UNICAST_BEACON, message->hdr.dst, message->hdr.src, message->hdr.shDst, message->hdr.shSrc);
      sendResponse = TRANSMIT_RESPONSE;
   }
   return sendResponse;
}

/**
 *message = source/destination.
 */
uint8_t link_layer_form_packet(dogePacket* packet, packetAttr* attr, uint8_t type, uint16_t src, uint16_t dst, uint16_t shSrc, uint16_t shDst)
{
   memset(packet, 0, sizeof(packet->hdr));
   SET_HEADER_TYPE(packet->hdr.type, type);
   SET_HEADER_TYPE_ACK(packet->hdr.type, attr->ack);
   packet->hdr.src = src;
   packet->hdr.dst = dst;
   packet->hdr.shSrc = shSrc;
   packet->hdr.shDst = shDst;
   packet->hdr.ttl = DEFAULT_PACKET_TTL;
   if (type == RAW_PACKET)
   {
      ((rawPacket*)packet)->size = attr->size;
   }
   else if(IS_HEADER_TYPE_ACK(packet->hdr.type))
   {
      ((packetAck*)packet)->errorCode = 0;
   }
   return 0;
}

uint8_t application_parse_packet(struct Protocol* obj, appPacket* message, appPacket* response, packetAttr* messageAttr, packetAttr* responseAttr)
{
   uint8_t returnCode = 0;
   uint8_t returnData = 0;

   switch(message->cmd){
      case(CMD_READ_REG): //read_reg
         if(message->addr >= MM_FUNCTION_MAX){ //if address is beyond entire memory map
            application_form_packet(response, responseAttr, CMD_NACK, message->addr, ERR_RANGE, NULL);
         }else{
            if(message->addr < MM_PHYSICAL_MAX){ 
               returnData = obj->dataRegisters[message->addr];
            }else{
               check_mm_space(1, message->addr, &returnData, 0);
            }
            application_form_packet(response, responseAttr, CMD_ACK, message->addr, returnData, NULL);
         }
         returnCode = 1;
         break;

      case(CMD_WRITE_REG):
         if(message->addr >= MM_FUNCTION_MAX){ //if address is beyond entire memory map
            application_form_packet(response, responseAttr, CMD_NACK, message->addr, ERR_RANGE, NULL);
         }else{
            if(message->addr < MM_PHYSICAL_RW_BAR){ //if trying to write to read-only range
               application_form_packet(response, responseAttr, CMD_NACK, message->addr, RO_REGISTER);
            }else if(message->addr < MM_PHYSICAL_MAX){
               obj->dataRegisters[message->addr] = message->data;
            }else{
               check_mm_space(0, message->addr, &message->data, 0);
            }
            application_form_packet(response, responseAttr, CMD_ACK, message->addr, message->data, NULL);
         }
         returnCode = 1;
         break;
 
      case(CMD_READ_MEM): //READ_MEM
         if(message->addr >= MM_PHYSICAL_MAX || (message->addr + message->data) > MM_PHYSICAL_MAX ||
            message->data > MAX_CMD_READ_MEM_DATA_SIZE){
            application_form_packet(response, responseAttr, CMD_NACK, message->addr, ERR_RANGE, NULL);
         }else{
            application_form_packet(response, responseAttr, CMD_READ_MEM_ACK, message->addr, message->data, (uint8_t*)&obj->dataRegisters[message->addr]);
         }
         returnCode = 1;
         break;

      case(CMD_WRITE_MEM): //WRITE_MEM
         if(message->addr >= MM_PHYSICAL_MAX || (message->addr + message->data) > MM_PHYSICAL_MAX ||
            message->data > MAX_CMD_WRITE_MEM_DATA_SIZE){
            application_form_packet(response, responseAttr, CMD_NACK, message->addr, ERR_RANGE, NULL);
         }else{
            copy_bytes((uint8_t*)&obj->dataRegisters[message->addr], CMD_WRITE_MEM_DATA_ADDRESS(message), message->data);
            application_form_packet(response, responseAttr, CMD_WRITE_MEM_ACK, message->addr, message->data, NULL);
         }
         returnCode = 1;
         break;

      case(CMD_ACK): //ack
         returnCode = 0;
         break;

      case(CMD_NACK): //nack
         returnCode = 0;
         break;

      case(CMD_NOP): //nop
         returnCode = 0;
         break;
      
      case(CMD_READ_MEM_ACK):
      case(CMD_WRITE_MEM_ACK):
         returnCode = 0;
         break;

      default: //unknown command
         application_form_packet(response, responseAttr, CMD_NACK, message->cmd, ERR_COMMAND, NULL);
         returnCode = 1;
         break;
   }//end switch
   return returnCode;
}//end application_parse_packet

uint8_t application_form_packet(appPacket* packet, packetAttr* attr, uint8_t cmd, uint8_t addr, uint8_t data, uint8_t* bytes){
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
      case(CMD_READ_MEM):
         packet->addr = (addr);
         packet->data = (data);
         attr->size = CMD_READ_MEM_DATA_SIZE;
         break;
      case(CMD_WRITE_MEM):
         packet->addr = (addr);
         packet->data = (data);
         copy_bytes(&packet->byteNumber, bytes, packet->data);
         attr->size = CMD_WRITE_MEM_DATA_SIZE(data);
         break;
      case(CMD_READ_MEM_ACK):
         packet->addr = (addr);
         copy_bytes(&packet->data, bytes, data);
         attr->ack = TRUE;
         attr->size = CMD_READ_MEM_ACK_DATA_SIZE(data);
         break;
      case(CMD_WRITE_MEM_ACK):
         packet->addr = (addr);
         packet->data = (data);
         attr->ack = TRUE;
         attr->size = CMD_WRITE_MEM_ACK_DATA_SIZE;
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

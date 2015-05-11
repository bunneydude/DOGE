#include "reliable_channel.h"

extern dogePacket rxPacket;
extern dogePacket txPacket;
extern packetAttr txAttr;
extern packetAttr rxAttr;

dogeBool reliable_transmit(uint8_t radioID)
{
  static uint8_t txPacketId = 0;
  uint8_t nextPacketId      = txPacketId;
  uint8_t retryCount        = 0;
  uint8_t txCount           = 0;
  dogeBool transmit         = TRUE;
  dogeTimer retryTimer      = {0};
  dogeTimer txTimer         = {0};
  timer_init(&txTimer, RETRY_TIMER_INTERVAL*(RETRY_COUNT + 1));
  while(nextPacketId == txPacketId){
    if (transmit){
      transmit = FALSE;
      SET_TXINFO_PACKET_ID(txPacket.hdr.txInfo, nextPacketId);
      add_packet_crc(&txPacket);
      dogeRadios[radioID].send_data(DOGE_BROADCAST, (uint8_t*)(&txPacket), RAW_PACKET_TOTAL_SIZE(&txPacket));
      while(dogeRadios[radioID].sending());
      timer_init(&retryTimer, RETRY_TIMER_INTERVAL);
    }
    else if( dogeRadios[radioID].get_data((uint8_t*)(&rxPacket), MAX_DATA_LENGTH, RADIO_RX_TIMEOUT) > 0){
      if (IS_HEADER_TYPE_ACK(rxPacket.hdr.type) &&
          HEADER_TYPE_EQUALS(rxPacket.hdr.type, LINK_LAYER_PACKET) &&
          TXINFO_PACKET_ID_EQUALS(rxPacket.hdr.txInfo, nextPacketId) &&
          rxPacket.hdr.shSrc == txPacket.hdr.shDst){
        transmit = TRUE;
        txCount++;
        nextPacketId++;
      }
    }
    else if(timer_expired(&retryTimer)){
      if (retryCount == RETRY_COUNT){ //reached max retry attempts
        /*print_string("Exceeded retry attempts", NEWLINE);*/
        break;
      }
      retryCount++;
      /*print_string("Retry: ", NONE);*/
      /*print_decimal(retryCount, NEWLINE);*/
      SET_TXINFO_RTA(txPacket.hdr.txInfo, retryCount);
      add_packet_crc(&txPacket);
      /*print_packet(&txPacket);*/
      dogeRadios[radioID].send_data(DOGE_BROADCAST, (uint8_t*)(&txPacket), RAW_PACKET_TOTAL_SIZE(&txPacket));
      while (dogeRadios[radioID].sending());
      timer_init(&retryTimer, RETRY_TIMER_INTERVAL);
    }
  }
  while(!timer_expired(&txTimer));
  if(txPacketId != nextPacketId){
    txPacketId = nextPacketId;
  }
  else{
    txPacketId++;
  }
  if (txCount == 1){ //TODO: support more than 1 packet
    return TRUE;
  }
  else{
    return FALSE;
  }
}

dogeBool reliable_receive(uint16_t timeout, uint8_t radioID)
{
  uint8_t rxPacketId      = 0;
  uint8_t nextPacketId    = 0;
  uint16_t shSrc          = 0;
  dogeBool firstPacket    = TRUE;
  dogeBool parsePacket    = FALSE;
  dogeTimer rxTimer       = {0};
  dogePacket queuedPacket = {{0}, {0}};

  timer_init(&rxTimer, timeout);
  while(!timer_expired(&rxTimer)){
    if (dogeRadios[radioID].get_data((uint8_t*)(&queuedPacket), MAX_DATA_LENGTH, RADIO_RX_TIMEOUT) > 0){
      /* Reject the message if the CRC is invalid or we can't forward or parse the message */
      /*print_packet(&queuedPacket);*/
      if(check_packet_crc(&queuedPacket) != SUCCESS || MY_NODE_ID != queuedPacket.hdr.shDst){ 
        /*print_string("ERROR", NEWLINE);*/
        continue;
      }
      if (firstPacket)
      {
        /*
        * 1. Assign rx packet ids based on the first packet received
        * 2. Tie this receiving period to the single hop source
        * 3. Reset rxTimer to RX_TIMER_TIMEOUT
        */
        firstPacket = FALSE;
        rxPacketId = GET_TXINFO_PACKET_ID(queuedPacket.hdr.txInfo);
        nextPacketId = rxPacketId;
        shSrc = queuedPacket.hdr.shSrc;
        timer_init(&rxTimer, RX_TIMER_TIMEOUT);
      }
      /* Ignore the packet if we are already communicating with a different node */
      if (shSrc != queuedPacket.hdr.shSrc){
        continue;
      }
      if (rxPacketId == nextPacketId){
        parsePacket = TRUE;
        txAttr.ack = TRUE;
        txAttr.size = 0;
        /* Form and send link layer acknowledgement */
        link_layer_form_packet(&txPacket, &txAttr, LINK_LAYER_PACKET, MY_NODE_ID, shSrc, MY_NODE_ID, shSrc);
        SET_TXINFO_PACKET_ID(txPacket.hdr.txInfo, nextPacketId);
        add_packet_crc(&txPacket);
        /*print_string("Transmitting ACK", NEWLINE);*/
        /*print_packet(&txPacket);*/
        dogeRadios[radioID].send_data(DOGE_BROADCAST, (uint8_t*)(&txPacket), sizeof(packetAck));
        /* Copy packet while radio is busy */
        copy_doge_packet(&rxPacket, &queuedPacket);
        while (dogeRadios[radioID].sending());
        nextPacketId++;
      }
      else{
        txAttr.ack = TRUE;
        txAttr.size = 0;
        link_layer_form_packet(&txPacket, &txAttr, LINK_LAYER_PACKET, MY_NODE_ID, shSrc, MY_NODE_ID, shSrc);
        SET_TXINFO_PACKET_ID(txPacket.hdr.txInfo, nextPacketId - 1);
        add_packet_crc(&txPacket);
        dogeRadios[radioID].send_data(DOGE_BROADCAST, (uint8_t*)(&txPacket), sizeof(packetAck));
        while (dogeRadios[radioID].sending());
        if (GET_TXINFO_RTA(rxPacket.hdr.txInfo) == RETRY_COUNT)
        {
          break;
        }
      }
    }
  }
  return parsePacket;
}

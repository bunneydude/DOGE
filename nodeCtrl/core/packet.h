#ifndef PACKET_H
#define PACKET_H
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if DBG
#define static_assert1(cond) static uint8_t static_assert1[((cond) == 1) ? 1 : -1]
#define static_assert2(cond) static uint8_t static_assert2[((cond) == 1) ? 1 : -1]
#define static_assert3(cond) static uint8_t static_assert3[((cond) == 1) ? 1 : -1]
#define static_assert4(cond) static uint8_t static_assert4[((cond) == 1) ? 1 : -1]
#define static_assert5(cond) static uint8_t static_assert5[((cond) == 1) ? 1 : -1]
#else
#define static_assert1(cond)
#define static_assert2(cond)
#define static_assert3(cond)
#define static_assert4(cond)
#define static_assert5(cond)
#endif

#define MAX_DATA_LENGTH 32
#define MAX_RAW_PACKET_PAYLOAD_SIZE 19
#define MAX_PACKET_PAYLOAD_SIZE     20

typedef enum {
   UNDEFINED_PACKET_TYPE = 0,
   RAW_PACKET,
   SIGNALING_BROADCAST_BEACON,
   SIGNALING_UNICAST_BEACON,
   BULK_DATA_READ_REQ,
   BULK_DATA_READ_DATA,
   BULK_DATA_WRITE_REQ,
   BULK_DATA_WRITE_DATA,
   BULK_DATA_RW_ABORT,
   LINK_LAYER_PACKET,
   MAX_PACKET_TYPE = 10
} packetType;

typedef enum {
  NO_TRANSMIT = 0,
  TRANSMIT_RESPONSE = 1
} responseStatus;

static_assert1(LINK_LAYER_PACKET < MAX_PACKET_TYPE);

typedef struct {
   uint8_t type;
   uint8_t txInfo;
   uint16_t src;
   uint16_t dst;
   uint16_t shSrc;
   uint16_t shDst;
   uint8_t ttl;
   uint8_t crc;
} packetHdr;

static_assert2(sizeof(packetHdr) == 12);

typedef struct {
   packetHdr hdr;
   uint8_t size;
   uint8_t data[MAX_RAW_PACKET_PAYLOAD_SIZE];
} rawPacket;

static_assert3(sizeof(rawPacket) == 32);

#pragma pack(2)
typedef struct {
   packetHdr hdr;
   uint8_t errorCode;
} packetAck;
#pragma pack()

static_assert4(sizeof(packetAck) == 14);

typedef struct {
   packetHdr hdr;
   uint8_t payload[MAX_PACKET_PAYLOAD_SIZE];
} dogePacket;

static_assert5(sizeof(dogePacket) == sizeof(rawPacket));

#define HEADER_TYPE_ACK_MASK       0x80
#define HEADER_TYPE_ACK_SHIFT      7
#define HEADER_TYPE_MASK           0x7F
#define HEADER_TYPE_SIZE_MASK      0x7F
#define HEADER_TYPE_SHIFT          0
#define TXINFO_PACKET_ID_MASK      0xF0
#define TXINFO_PACKET_ID_SHIFT     4
#define TXINFO_PACKET_ID_SIZE_MASK 0xF
#define TXINFO_RTA_MASK            0x0F
#define TXINFO_RTA_SHIFT           0
#define TXINFO_RTA_SIZE_MASK       0xF

/** Macro for setting ack in header type */
#define SET_HEADER_TYPE_ACK(TYPE, ACK) \
   do { \
      (TYPE) &= ~(HEADER_TYPE_ACK_MASK); \
      (TYPE) |= ACK << (HEADER_TYPE_ACK_SHIFT); \
   } \
while(0)

/** Macro for setting type in header type */
#define SET_HEADER_TYPE(HEADER_TYPE, TYPE) \
   do { \
      (HEADER_TYPE) &= ~(HEADER_TYPE_MASK); \
      (HEADER_TYPE) |= ((TYPE) & HEADER_TYPE_SIZE_MASK) << (HEADER_TYPE_SHIFT); \
   } \
while(0)

/** Macro for setting packet ID in header txinfo */
#define SET_TXINFO_PACKET_ID(TXINFO, PACKET_ID) \
   do { \
      (TXINFO) &= ~(TXINFO_PACKET_ID_MASK); \
      (TXINFO) |= ((PACKET_ID) & TXINFO_PACKET_ID_SIZE_MASK) << (TXINFO_PACKET_ID_SHIFT); \
   } \
while(0)

/** Macro for setting RTA in header txinfo */
#define SET_TXINFO_RTA(TXINFO, RTA) \
   do { \
      (TXINFO) &= ~(TXINFO_RTA_MASK); \
      (TXINFO) |= ((RTA) & TXINFO_RTA_SIZE_MASK) << (TXINFO_RTA_SHIFT); \
   } \
while(0)

/** Macro for getting type in header type */
#define GET_HEADER_TYPE(HEADER_TYPE) ((((HEADER_TYPE) & HEADER_TYPE_MASK) >> HEADER_TYPE_SHIFT))

/** Macro for getting ack in header type */
#define GET_HEADER_TYPE_ACK(TYPE) (((TYPE) & HEADER_TYPE_ACK_MASK) >> HEADER_TYPE_ACK_SHIFT)

/** Macro for getting packet ID in header txinfo */
#define GET_TXINFO_PACKET_ID(_TXINFO_) (((_TXINFO_) & TXINFO_PACKET_ID_MASK) >> (TXINFO_PACKET_ID_SHIFT))

/** Macro for getting RTA in header txinfo */
#define GET_TXINFO_RTA(TXINFO) (((TXINFO) & TXINFO_RTA_MASK) >> (TXINFO_RTA_SHIFT))

/** Macro for testing header ACK type */
#define IS_HEADER_TYPE_ACK(TYPE) (((TYPE) & HEADER_TYPE_ACK_MASK) == HEADER_TYPE_ACK_MASK)

/** Macro for testing header type */
#define HEADER_TYPE_EQUALS(HEADER_TYPE, TYPE) ((((HEADER_TYPE) & HEADER_TYPE_MASK) >> HEADER_TYPE_SHIFT) == (TYPE))

/** Macro for testing packet id */
#define TXINFO_PACKET_ID_EQUALS(TXINFO, PACKET_ID) (GET_TXINFO_PACKET_ID(TXINFO) == (PACKET_ID & TXINFO_PACKET_ID_SIZE_MASK))

#define NUM_PACKET_HEADER_CRC_BYTES        (sizeof(packetHdr) - sizeof(((packetHdr*)0)->crc))
#define RAW_PACKET_DATA_OFFSET             (offsetof(rawPacket, data))
#define RAW_PACKET_DATA_CRC_BEGIN          (RAW_PACKET_DATA_OFFSET - sizeof(((rawPacket*)0)->size))
#define RAW_PACKET_DATA_CRC_END(_packet_)  (RAW_PACKET_DATA_CRC_BEGIN + sizeof(((rawPacket*)0)->size) + (_packet_)->size)
#define RAW_PACKET_TOTAL_SIZE(_packet_)    (sizeof(packetHdr) + sizeof(((rawPacket*)0)->size) + ((rawPacket*)(_packet_))->size)
#define RAW_PACKET_DATA_SIZE(_packet_)     (((rawPacket*)(_packet_))->size)
#define PACKET_PAYLOAD_OFFSET              (offsetof(dogePacket, payload))
#define CMD_READ_MEM_DATA_ADDRESS(_packet_) (&((appPacket*)(_packet_))->data)
#define CMD_WRITE_MEM_DATA_ADDRESS(_packet_) (&((appPacket*)(_packet_))->byteNumber)
#define DEFAULT_PACKET_TTL (255)

void copy_bytes(uint8_t* dst, uint8_t* src, uint8_t size);
void copy_doge_packet(dogePacket* dst, dogePacket* src);
void copy_raw_packet_data (rawPacket* dst, rawPacket* src);
uint8_t packet_payload_end(dogePacket* packet);
void add_packet_crc(dogePacket* packet);
uint8_t check_packet_crc(dogePacket* packet);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

#ifndef PACKET_H
#define PACKET_H
#include <stdint.h>

#if DBG
#define static_assert1(cond) uint8_t static_assert1[((cond) == 1) ? 1 : -1]
#define static_assert2(cond) uint8_t static_assert2[((cond) == 1) ? 1 : -1]
#define static_assert3(cond) uint8_t static_assert3[((cond) == 1) ? 1 : -1]
#define static_assert4(cond) uint8_t static_assert4[((cond) == 1) ? 1 : -1]
#else
#define static_assert1(cond)
#define static_assert2(cond)
#define static_assert3(cond)
#define static_assert4(cond)
#endif

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
   MAX_PACKET_TYPE = 9
} packetType;

static_assert1(BULK_DATA_RW_ABORT < MAX_PACKET_TYPE);

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
#define GET_HEADER_TYPE(HEADER_TYPE, TYPE) \
   do { \
      (TYPE) = ((HEADER_TYPE) & HEADER_TYPE_MASK) >> HEADER_TYPE_SHIFT; \
   } \
while(0)

/** Macro for getting ack in header type */
#define GET_HEADER_TYPE_ACK(TYPE, ACK) \
   do { \
      (ACK) = ((TYPE) & HEADER_TYPE_ACK_MASK) >> HEADER_TYPE_ACK_SHIFT; \
   } \
while(0)

/** Macro for getting packet ID in header txinfo */
#define GET_TXINFO_PACKET_ID(TXINFO, PACKET_ID) \
   do { \
      (PACKET_ID) = ((TXINFO) & TXINFO_PACKET_ID_MASK) >> (TXINFO_PACKET_ID_SHIFT); \
   } \
while(0)

/** Macro for getting RTA in header txinfo */
#define GET_TXINFO_RTA(TXINFO, RTA) \
   do { \
      (RTA) = ((TXINFO) & TXINFO_RTA_MASK) >> (TXINFO_RTA_SHIFT); \
   } \
while(0)

#define MAX_RAW_PACKET_PAYLOAD_SIZE 23

typedef struct {
   uint8_t type;
   uint8_t txInfo;
   uint16_t src;
   uint16_t dst;
   uint8_t ttl;
   uint8_t crc;
} packetHdr;
static_assert2(sizeof(packetHdr) == 8);

typedef struct {
   packetHdr hdr;
   uint8_t size;
   uint8_t data[MAX_RAW_PACKET_PAYLOAD_SIZE];
} rawPacket;
static_assert3(sizeof(rawPacket) == 32);

#define NUM_PACKET_HEADER_CRC_BYTES (sizeof(packetHdr) - sizeof(((packetHdr*)0)->crc))
#define RAW_PACKET_DATA_OFFSET      (offsetof(rawPacket, data))

#pragma pack(2)
typedef struct {
   packetHdr hdr;
   uint8_t errorCode;
} packetAck;
#pragma pack()
static_assert4(sizeof(packetAck) == 10);

void add_raw_packet_crc(rawPacket* packet);
uint8_t check_raw_packet_crc(rawPacket* packet);

#endif

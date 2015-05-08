#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include "../memory_map/memory_map.h"
#include "../nodeCtrl_errno.h"
#include "../packet.h"
#include <type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if DBG
#define static_assert5(cond) uint8_t static_assert5[((cond) == 1) ? 1 : -1]
#define static_assert6(cond) uint8_t static_assert6[((cond) == 1) ? 1 : -1]
#else
#define static_assert5(cond)
#define static_assert6(cond)
#endif

#define MAX_PAYLOAD_SIZE (14)
#define MAX_CMD_READ_MEM_DATA_SIZE (12)
#define MAX_CMD_WRITE_MEM_DATA_SIZE (12)

#define CMD_READ_REG_DATA_SIZE           (2)
#define CMD_WRITE_REG_DATA_SIZE          (3)
#define CMD_ACK_DATA_SIZE                (3)
#define CMD_NACK_DATA_SIZE               (3)
#define CMD_NOP_DATA_SIZE                (1)
#define CMD_READ_MEM_DATA_SIZE           (3)
#define CMD_READ_MEM_ACK_DATA_SIZE(size) ((size) + 2)
#define CMD_WRITE_MEM_DATA_SIZE(size)    ((size) + 3)
#define CMD_WRITE_MEM_ACK_DATA_SIZE      (3)

//commands
enum Protocol_commands{
   CMD_READ_REG = 0x1,
   CMD_WRITE_REG = 0x2,
   CMD_ACK = 0x3,
   CMD_NACK = 0x4,
   CMD_NOP = 0x5,
   CMD_READ_MEM = 0x6,
   CMD_WRITE_MEM = 0x7,
   CMD_ADC_SINGLE = 0x8,
   CMD_ADC_RESULT = 0x9,
   CMD_ADC_LOOP = 0xA,
   CMD_ADC_END = 0xB,
   CMD_SPI = 0xC,
   CMD_READ_MEM_ACK = 0xD,
   CMD_WRITE_MEM_ACK = 0xE
};

//error codes
enum Protocol_errors{
   ERR_CHECKSUM    = ERROR_PROTOCOL_BASE + 0x1,
   ERR_COMMAND     = ERROR_PROTOCOL_BASE + 0x2,
   ERR_FEATURE_WIP = ERROR_PROTOCOL_BASE + 0x3,
   ERR_CAT         = ERROR_PROTOCOL_BASE + 0x4,
   ERR_RANGE       = ERROR_PROTOCOL_BASE + 0x5,
   ERR_TIMEOUT     = ERROR_PROTOCOL_BASE + 0x6
};

struct Protocol{
	uint8_t* dataRegisters;
};

#pragma pack(2)
typedef struct {
   uint8_t cmd;
   uint8_t addr;
   uint8_t data;
   uint8_t byteNumber; //unused
   uint8_t payload[MAX_PAYLOAD_SIZE];
} appPacket;
#pragma pack()

typedef struct {
   uint8_t ack;
   uint8_t size;
   //could also shove source and destination node ids in here
} packetAttr;

static_assert5(sizeof(appPacket) == 18);
static_assert6((sizeof(((rawPacket*)0)->hdr) +
                sizeof(((rawPacket*)0)->size) +
                sizeof(appPacket)) <= sizeof(rawPacket));

void Protocol_init(struct Protocol* obj);
uint8_t link_layer_parse_packet(struct Protocol* obj, dogePacket* message, dogePacket* response);
uint8_t link_layer_form_packet(dogePacket* packet, packetAttr* attr, uint8_t type, uint16_t src, uint16_t dst, uint16_t shSrc, uint16_t shDst);
uint8_t application_parse_packet(struct Protocol* obj, appPacket* message, appPacket* response, packetAttr* messageAttr, packetAttr* responseAttr);
uint8_t application_form_packet(appPacket* packet, packetAttr* attr, uint8_t cmd, uint8_t addr, uint8_t data);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

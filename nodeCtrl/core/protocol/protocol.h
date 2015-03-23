#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include "../memory_map/memory_map.h"
#include "../nodeCtrl_errno.h"

#define MAX_PAYLOAD_SIZE 16

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
	CMD_SPI = 0xC
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
	uint8_t cmd;
	uint8_t size;
	uint8_t addr;
	uint8_t data;
	uint8_t checksum;
	uint8_t byteNumber;
	uint8_t payload[MAX_PAYLOAD_SIZE];
	uint8_t executePacket;
	uint8_t* dataRegisters;
};

void Protocol_init(struct Protocol* obj);

uint8_t Protocol_form_packet(uint8_t* buf, uint8_t cmd, uint8_t addr, uint8_t data);
uint8_t Protocol_parse_packet(struct Protocol* obj, uint8_t* buf, uint8_t* response);

#endif

#ifndef NODECTRL_H
#define NODECTRL_H

#include <stdint.h>


union attr16{
	uint16_t val16;
	struct {uint8_t b0, b1;} val8;
};

enum nodeCtrlCommands{
	CMD_READ_REG = 0x1,
	CMD_WRITE_REG = 0x2,
	CMD_ACK = 0x3,
	CMD_NACK = 0x4,
	CMD_NOP = 0x5,

	CMD_PULL_ENABLE,
	CMD_PULL_DIR,
	CMD_PIN_OUTPUT,
	CMD_PIN_DIR,
	CMD_PIN_VALUE,

	CMD_STATUS,
	CMD_DEVICE
};

enum nodeCtrlErrors{
	ERR_CHECKSUM = 0x1,
	ERR_COMMAND = 0x2,
	ERR_FEATURE_WIP = 0x3,
	ERR_CAT = 0x4,
	ERR_RANGE = 0x5,
	ERR_TIMEOUT = 0x6
};

struct nodeCtrl{
	uint8_t cmd;
	uint8_t size;
	uint8_t addr;
	uint8_t data;
	uint8_t byteNumber;
//	uint8_t payload[RING_SIZE];
	uint8_t executePacket;
	uint8_t returnData;
	uint8_t special;
//	uint8_t dataRegisters[SERIAL_REGISTER_NUM];
//	volatile uint8_t uartTxData[RING_SIZE];
};

void nodeCtrl_init(struct nodeCtrl *obj);
uint8_t nodeCtrl_form_packet(uint8_t *buf, uint8_t cmd, uint8_t addr, uint8_t data);
uint8_t nodeCtrl_parse_packet(struct nodeCtrl *obj, uint8_t *buf, uint8_t *response);

/* Implement these methods on your own */
extern void nodeCtrl_checksum(uint8_t *buf, uint8_t length);
extern void nodeCtrl_frame(uint8_t *buf, uint8_t length);

#endif

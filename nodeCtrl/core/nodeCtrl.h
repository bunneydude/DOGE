/** @file nodeCtrl.h Contains Node Control structures, enumerations, and function prototypes  */
#ifndef NODECTRL_H
#define NODECTRL_H

#ifdef MSP430
#include <stdint.h>
#include <memory_map.h>
#include <network.h>
#include <protocol.h>
#include <neighbor-config.h>
#include <neighbor.h>
#include <gpio.h>
#include <msp430_gpio.h>
#include <adc.h>
#include <SPI.h>
#include <AIR430BoostFCC.h>
#include <dsp.h>
#include <platform.h>
#include <nrfLegacy.h>
#endif

/** @brief 16 bit data type with 2 bytes */
union attr16{
	uint16_t val16;
	struct {uint8_t b0, b1;} val8;
};

/*
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
*/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void nodeCtrl_init();
void nodeCtrl_entry() __attribute__ ((noreturn));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

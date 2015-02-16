#include <stdint.h>
#include "nodeCtrl.h"

void nodeCtrl_init(struct nodeCtrl *obj)
{
	obj->byteNumber = 0;
	obj->cmd = 0;
	obj->addr = 0;
	obj->returnData = 0;
	obj->executePacket = 0;
}

//uint8_t nodeCtrl_parse_packet(uint8_t *buf, uint8_t length, struct packet

uint8_t nodeCtrl_form_packet(uint8_t *buf, uint8_t cmd, uint8_t addr, uint8_t data)
{
	uint8_t index = 0;

	buf[index++] = cmd;
	buf[index++] = 0; /*size of payload, fill in later*/
	

}

/*
 * pin named by 0-255 
 * To set multiple pins, specify starting pin, 8-bit mask, then the values.
 * e.g. set pins 20, 22, 23, 24, 26: high, low, low, high, low
 * 	CMD = CMD_PIN_OUTPUT
 *      SUBCMD = multiple
 *	start pin = d20
 *	mask = 0b01011101
 * 	values = 0b01001 (upper 3 bits are ignored)


   Set just pin 20 high
	CMD = CMD_PIN_OUTPUT
	SUBCMD = single
	pin = d20
	value = 1

   Alt to above: adds a bye, but using a subcmd adds a byte as well
	CMD = CMD_PIN_OUTPUT
	SUBCMD = multiple
	start pin = d20
	mask = 0x1
	values = 0x1

*/

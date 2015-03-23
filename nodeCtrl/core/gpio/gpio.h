#ifndef GPIO_H
#define GPIO_H

#include "../nodeCtrl_errno.h"

/** @brief Handles reads and writes to the memmory-mappped
 *         function section for GPIO.
 *
 *  When a memory-mapped function register is accessed
 *  this method will call a function based on the addr
 *  field.
 *
 *  @param rw If the command was a read (1) or write(0).
 *  @param addr The memory-map address specified.
 *  @param data The value writted the the register. Ignored for reads.
 *  @param mask For a write, if bit n is set in mask, then bit n of 
 *              the register will not be altered. For a read, bit n
 *              of the result will always be 0.
 *  @return The return data if a read was requested.
 */
uint8_t gpio_mm_handler(uint8_t rw, uint8_t addr, uint8_t* data, uint8_t mask);


enum gpioErrorCodes{
	RO_REGISTER         = ERROR_GPIO_BASE + 0x0,
	WO_REGISTER         = ERROR_GPIO_BASE + 0x1,
	ADDR_OUT_OF_RANGE   = ERROR_GPIO_BASE + 0x2,
	UNSUPPORTED_FEATURE = ERROR_GPIO_BASE + 0x3
};

#endif
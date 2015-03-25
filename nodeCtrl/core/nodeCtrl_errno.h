#ifndef NODECTRL_ERRNO_H
#define NODECTRL_ERRNO_H

enum nodeCtrlErrorBases{
	ERROR_PROTOCOL_BASE = 0x0,  //7
	ERROR_GPIO_BASE     = 0x8,  //4
	ERROR_ADC_BASE      = 0xC,  //4
	ERROR_RADIO_BASE    = 0x10, // 2-4?
	ERROR_MEMORY_MAP_BASE = 0x14 //4?
};

#endif

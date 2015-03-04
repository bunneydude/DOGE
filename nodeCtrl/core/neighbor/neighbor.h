#ifndef NEIGHBOR_H
#define NEIGHBOR_H

#include <stdint.h>
#include "neighbor-config.h"

#if PACK_STRUCT == 1
struct neighborEntry{
	uint16_t shNodeID;
	uint16_t shLQE : 12;
	uint8_t nodeID : 2;
	uint8_t networkID : 2;
};
#else
struct neighborEntry{
	uint16_t shNodeID;
	uint8_t nodeID;
	uint8_t networkID;
	uint16_t shLQE;
};
#endif

#ifdef LINUX
#pragma message("Linux target")
#else
	#ifdef MSP430
	#pragma message("MSP430 target")
	#else
	#pragma error("No target defined")
	#endif
#endif




#endif

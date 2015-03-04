#ifndef NEIGHBOR_H
#define NEIGHBOR_H

#include <stdint.h>
#include "neighbor-config.h"

//neighborEntry holds information needed to communicate w/ adjacent nodes
#if PACK_STRUCT == 1
struct neighborEntry{
	uint16_t shNodeID; //single-hop node ID; each node has a unique ID
	uint16_t shLQE : 12; //single-hop Link Quality Estimator; more details determined by the active LQE method
	uint8_t radioID : 2; //which radio this entry is for. Each node can support 4 radio links (don't have to all be different radios)
	uint8_t networkID : 2; //used for jumping between different RF networks. 
};
#else
struct neighborEntry{
	uint16_t shNodeID;
	uint16_t shLQE;
	uint8_t networkID;
	uint8_t radioID;
};
#endif

//Determine what we're trying to build for. Currently used to omit printfs
//might move this out to a more global file
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

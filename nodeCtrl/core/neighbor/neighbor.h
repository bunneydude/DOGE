/** @file neighbor.h Neighbor structure definitions */
#ifndef NEIGHBOR_H
#define NEIGHBOR_H

#include <stdint.h>
#include "neighbor-config.h"

/**
 * @brief Holds information needed to communicate w/ adjacent nodes
 */
//#if PACK_STRUCT == 1
#if 1
struct neighborEntry{
  /** @brief single-hop node ID; each node has a unique ID */
	uint16_t shNodeID; 
  /** @brief single-hop Link Quality Estimator; more details determined by the active LQE method */
	uint16_t shLQE : 12; 
  /** @brief which radio this entry is for. Each node can support 4 radio links (don't have to all be different radios) */
	uint8_t radioID : 2;
  /** @brief used for jumping between different RF networks. */ 
	uint8_t networkID : 2; 
};
#else
struct neighborEntry{
  /** @brief single-hop node ID; each node has a unique ID */
	uint16_t shNodeID;
  /** @brief single-hop Link Quality Estimator; more details determined by the active LQE method */
	uint16_t shLQE;
  /** @brief which radio this entry is for. Each node can support 4 radio links (don't have to all be different radios) */
	uint8_t networkID;
  /** @brief used for jumping between different RF networks. */ 
	uint8_t radioID;
};
#endif
#endif

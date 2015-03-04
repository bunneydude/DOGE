#ifndef ROUTING_H
#define ROUTING_H

#include <stdint.h>

//routingEntry holds information needed to route packets through the network
struct routingEntry{
	uint16_t mhNodeID; //multi-hop node ID; This is the final destination for the packet
	uint8_t mhLQE; //multi-hop Link Quality Estimator; Calculated over the entire path
	uint8_t neighborIndex; //index into the neighbor table where the packet should be sent next
};

#endif

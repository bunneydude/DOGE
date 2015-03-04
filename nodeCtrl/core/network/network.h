#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>
#include "../neighbor/neighbor.h"
#include "../routing/routing.h"

#define MAX_NETWORK_ENTRIES 15 //for now, can't be higher than 255
//TODO add preprocessor check for <255


union networkEntry{
	struct neighborEntry neighbor;
	struct routingEntry routing;
};

//Stores the current number of neighbor entries and routing entries in the network array
struct networkInfo{
	uint8_t maxNeighborEntries;
	uint8_t maxRoutingEntries;
};

//Single array to hold both neighbor and routing table entries
//This allows the max number of either entry type to change at runtime without wasting memory
//neighbor table entries are in the range [0, maxNeighborEntries]
//routing table entries are in the range [MAX_NETWORK_ENTRIES-1 - maxRoutingEntries, MAX_NETWORK_ENTRIES-1]
union networkEntry network[MAX_NETWORK_ENTRIES];
//TODO insert into memory map


//functions

//add an entry to network array
void insert(union networkEntry entry, uint8_t index);

//remove entry
//TODO might not be used. Allow mater node to remove specific entries.
//Local node may only need to reallocate
void delete(uint8_t index);

//shell-sort the neighbor and routing subsection of the network array based on sh/mh ID
void sort();

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
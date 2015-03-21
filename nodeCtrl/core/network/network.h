#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>
#include "../neighbor/neighbor.h"
#include "../routing/routing.h"
#include "../memory_map/memory_map.h"

#define MAX_NETWORK_ENTRIES 8 //for now, can't be higher than 255, 0-based indexing
#define NETWORK_DIVISION_DEFAULT 4

//TODO add preprocessor check for <255

enum networkEntryType{
	NEIGHBOR_ENTRY = 0,
	ROUTING_ENTRY = 1,
};

union networkEntry{
	struct neighborEntry neighbor;
	struct routingEntry routing;
};

//Stores the current number of neighbor entries and routing entries in the network array
struct networkIndices{
	uint8_t numberEntries[2]; //0 is neighbors, 1 is routing
	uint8_t divisionIndex; //a neighbor entry can't be inserted at or above this index; a routing entry can't be inserted lower than this index
};
//the largest allowed value for the division is MAX_NETWORK_ENTRIES and it cannot be 0. Otherwise there'd be room for only one routing table entry but no neighbor entry, which doesn't make any sense

//instead of using a division, we could steal a bit from an entry's LQE (11bit shLQE, 7bit mhLQE) and make a present bit
//we'd still want to know how many neighbor entries there are though...

//Single array to hold both neighbor and routing table entries
//This allows the max number of either entry type to change at runtime without wasting memory
//neighbor table entries are in the range [0, maxNeighborEntries]
//routing table entries are in the range [MAX_NETWORK_ENTRIES-1 - maxRoutingEntries, MAX_NETWORK_ENTRIES-1]
union networkEntry* network;//[MAX_NETWORK_ENTRIES];
struct networkIndices networkTable;

//TODO insert into memory map


//functions

void network_init(uint8_t division);

//add an entry to network array
uint8_t network_insert(union networkEntry* entry, enum networkEntryType type);

//remove entry
//TODO might not be used. Allow master node to remove specific entries.
//Local node may only need to reallocate
//void delete(uint8_t index);

//shell-sort the neighbor and routing subsection of the network array based on sh/mh ID
void sort();
#endif

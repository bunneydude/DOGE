#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>
#include "../neighbor/neighbor.h"
#include "../routing/routing.h"
#include "../memory_map/memory_map.h"
#include "../platform/serial_c.h"
#include "../protocol/type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_NETWORK_ENTRIES 8 //for now, can't be higher than 255, 0-based indexing
#define NETWORK_DIVISION_DEFAULT 4
//neighbor entries allowed in range [0, division)
//routing entries allowed in range [division, MAX_NETWORK_ENTRIES)

#define MASKED_LQE (0x00)
#define PERFECT_LQE (0xFF)
#define MAX_LQE (PERFECT_LQE - 1)
#define MIN_LQE (MASKED_LQE + 1)

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
struct networkControl{
   uint8_t numberEntries[2]; //0 is neighbors, 1 is routing
   uint8_t divisionIndex; //a neighbor entry can't be inserted at or above this index; a routing entry can't be inserted lower than this index
   uint8_t networkConfig;
};

enum networkConfigFields{ //8-bit field to control network behavior
   LQE_UPDATE = (1<<0), //if set, update LQE after getting a packet. Otherwise, don't update
   RSVD_1 = (1<<1),
   RSVD_2 = (1<<2),
   RSVD_3 = (1<<3),
   RSVD_4 = (1<<4),
   RSVD_5 = (1<<5),
   RSVD_6 = (1<<6),
   RSVD_7 = (1<<7),
};

//the largest allowed value for the division is MAX_NETWORK_ENTRIES and it cannot be 0. Otherwise there'd be room for only one routing table entry but no neighbor entry, which doesn't make any sense

//instead of using a division, we could steal a bit from an entry's LQE (11bit shLQE, 7bit mhLQE) and make a present bit
//we'd still want to know how many neighbor entries there are though...

//Single array to hold both neighbor and routing table entries
//This allows the max number of either entry type to change at runtime without wasting memory
//neighbor table entries are in the range [0, maxNeighborEntries]
//routing table entries are in the range [MAX_NETWORK_ENTRIES-1 - maxRoutingEntries, MAX_NETWORK_ENTRIES-1]
extern union networkEntry* network;
extern struct networkControl* networkInfo;

//TODO insert into memory map


//functions

void network_init(uint8_t division);
dogeBool network_has_neighbor(uint16_t id, uint8_t* index, uint8_t radioID, dogeBool includeMasked);
dogeBool network_has_route(uint16_t id, uint8_t* index, uint8_t radioID, dogeBool includeMasked);

//add an entry to network array
dogeBool network_insert(union networkEntry* entry, enum networkEntryType type);
uint8_t network_update(uint16_t id, uint8_t LQE, uint8_t radioId, uint8_t networkId, enum networkEntryType type);

//remove entry
//TODO might not be used. Allow master node to remove specific entries.
//Local node may only need to reallocate
//void delete(uint8_t index);

//shell-sort the neighbor and routing subsection of the network array based on sh/mh ID
void sort();
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

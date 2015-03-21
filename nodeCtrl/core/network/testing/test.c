#include <stdlib.h>
#include <stdio.h>
#include "network.h"

#ifdef LINUX 
void print_entry(union networkEntry e, enum networkEntryType type)
{
	printf("Number neighbor entries = %d, number routing entries = %d, division index = %d\n", networkTable.numberEntries[NEIGHBOR_ENTRY], networkTable.numberEntries[ROUTING_ENTRY], networkTable.divisionIndex);

	switch(type){
		case NEIGHBOR_ENTRY :
			printf("Neighbor entry:\n");
			printf("   shNodeID = %d\n", e.neighbor.shNodeID);
			printf("   shLQE = %d\n", e.neighbor.shLQE);
			printf("   radioID = %d\n", e.neighbor.radioID);
			printf("   networkID = %d\n\n", e.neighbor.networkID);
			break;
		case ROUTING_ENTRY : 
			printf("Routing entry:\n");
			printf("   mhNodeID = %d\n", e.routing.mhNodeID);
			printf("   mhLQE = %d\n", e.routing.mhLQE);
			printf("   neighborIndex = %d\n\n", e.routing.neighborIndex);
			break;
	}

}

#endif

void main(){
//init network

network_init(NETWORK_DIVISION_DEFAULT);


printf("N size = 0x%lu, R size = 0x%lu\n", sizeof(struct neighborEntry), sizeof(struct routingEntry));
printf("N entries = %d, R entries = %d\n", networkTable.numberEntries[NEIGHBOR_ENTRY], networkTable.numberEntries[ROUTING_ENTRY]);
printf("Entry division index = %d, max network entries = %d\n", networkTable.divisionIndex, MAX_NETWORK_ENTRIES);

union networkEntry nEntry;
union networkEntry rEntry;

nEntry.neighbor.shNodeID = 0xffff; //1;
nEntry.neighbor.shLQE = 0xfff; //6;
nEntry.neighbor.radioID = 0x3; //2;
nEntry.neighbor.networkID = 0x3; //3;

rEntry.routing.mhNodeID = 5;
rEntry.routing.mhLQE = 10;
rEntry.routing.neighborIndex = 1;

#ifdef LINUX
print_entry(nEntry, NEIGHBOR_ENTRY);
print_entry(rEntry, ROUTING_ENTRY);
#endif

network_insert(&nEntry, NEIGHBOR_ENTRY);
print_entry(nEntry, NEIGHBOR_ENTRY);

network_insert(&nEntry, NEIGHBOR_ENTRY);
print_entry(nEntry, NEIGHBOR_ENTRY);

network_insert(&nEntry, NEIGHBOR_ENTRY);
print_entry(nEntry, NEIGHBOR_ENTRY);

network_insert(&nEntry, NEIGHBOR_ENTRY);
print_entry(nEntry, NEIGHBOR_ENTRY);

network_insert(&nEntry, NEIGHBOR_ENTRY);
print_entry(nEntry, NEIGHBOR_ENTRY);

uint8_t i = 0;
for(i=0; i<16; i++){
	network_insert(&rEntry, ROUTING_ENTRY);
	print_entry(rEntry, ROUTING_ENTRY);
}
/*
   network_insert(&rEntry, ROUTING_ENTRY);

#ifdef LINUX
print_entry(network[0], NEIGHBOR_ENTRY);
print_entry(network[1], ROUTING_ENTRY);
#endif

network_insert(&rEntry, ROUTING_ENTRY);

#ifdef LINUX
print_entry(network[0], ROUTING_ENTRY);
#endif
*/
}

#include <stdlib.h>
#include <stdio.h>
#include "network.h"

#ifdef LINUX 

void print_contents(){
	uint8_t j = 0;
	uint8_t x;
printf("\n");
	for(j=1; j<=4; j++){
		printf("Network has neighbor %d: %d\n",j, network_has_neighbor(j, &x));
		printf("Network has route %d: %d\n",j, network_has_route(j, &x));
	}
}
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

uint8_t i =0;

void main(){
//init network

network_init(NETWORK_DIVISION_DEFAULT);

#ifdef LINUX
printf("N size = 0x%lu, R size = 0x%lu\n", sizeof(struct neighborEntry), sizeof(struct routingEntry));
printf("N entries = %d, R entries = %d\n", networkTable.numberEntries[NEIGHBOR_ENTRY], networkTable.numberEntries[ROUTING_ENTRY]);
printf("Entry division index = %d, max network entries = %d\n", networkTable.divisionIndex, MAX_NETWORK_ENTRIES);
#endif

union networkEntry nEntries[4];
union networkEntry rEntries[4];
uint8_t index;

for(i=0; i<4; i++){
	nEntries[i].neighbor.shNodeID = i+1;
	nEntries[i].neighbor.shLQE = 3;
	nEntries[i].neighbor.radioID = 2;
	nEntries[i].neighbor.networkID = 1;

	rEntries[i].routing.mhNodeID = i+1;
	rEntries[i].routing.mhLQE = 4;
	rEntries[i].routing.neighborIndex = 5;
}

network_insert(&nEntries[0], NEIGHBOR_ENTRY);
network_insert(&rEntries[0], ROUTING_ENTRY);

network[0].neighbor.shLQE = 1;
print_entry(nEntries[0], NEIGHBOR_ENTRY);

print_contents();

printf("Orig LQE = %d\n", network[0].neighbor.shLQE);
if(network_has_neighbor(1, &index)){
	network[index].neighbor.shLQE = 0x5;
}
printf("New LQE = %d\n", network[0].neighbor.shLQE);


printf("Orig LQE = %d\n", network[MAX_NETWORK_ENTRIES-1].routing.mhLQE);
if(network_has_route(1, &index)){
	network[index].routing.mhLQE = 0x6;
}
printf("New LQE = %d\n", network[MAX_NETWORK_ENTRIES-1].routing.mhLQE);
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

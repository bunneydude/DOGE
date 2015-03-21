#include <stdio.h>
#include "network.h"

void network_init(uint8_t division)
{
network = ( (union networkEntry*)(&(memoryMap[MM_NETWORK_BASE].u8[0])));

networkTable.numberEntries[NEIGHBOR_ENTRY] = 0;
networkTable.numberEntries[ROUTING_ENTRY] = 0;
networkTable.divisionIndex = 4;


}



uint8_t network_room_check(enum networkEntryType type)
{
	//TODO could combine the first two ifs
	if( (type == NEIGHBOR_ENTRY) && (networkTable.numberEntries[NEIGHBOR_ENTRY] < networkTable.divisionIndex) ){
		return 1; //room for another neighbor entry
	}else if( (type == ROUTING_ENTRY) && ( (MAX_NETWORK_ENTRIES-1) - networkTable.numberEntries[ROUTING_ENTRY] >= networkTable.divisionIndex) ){
		return 1; //room for another routing entry
	}else{
		return -1;
	}
}

uint8_t network_insert(union networkEntry* entry, enum networkEntryType type)
{
	uint8_t roomLeft = 0;
	roomLeft = network_room_check(type);
	if(roomLeft == 1){
		printf("Room left for type %d\n", type);	
	//this clear currently isn't strictly needed since both structs inside the union have no padding bits...but this might not be the case forever. Also have to make sure there aren't compiler-specific differences w/ sizing the structs
	//*((uint32_t*)(&network[index])) = 0;		
		network[networkTable.numberEntries[type]++] = *entry;
		return 1;
	}else{
		printf("No room left for type %d\n", type);	
		return -1;
	}
}

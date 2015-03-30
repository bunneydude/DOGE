#include <stdio.h>
#include "network.h"

union networkEntry* network;
struct networkIndices networkTable;

void network_init(uint8_t division)
{
	network = ( (union networkEntry*)(&(memoryMap[MM_NETWORK_BASE/4].u8[MM_NETWORK_BASE%4])));

	networkTable.numberEntries[NEIGHBOR_ENTRY] = 0;
	networkTable.numberEntries[ROUTING_ENTRY] = 0;
	networkTable.divisionIndex = division;


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
//returns 1 for success, 0 otherwise
uint8_t network_has_neighbor(uint16_t id, uint8_t* index)
{
	uint8_t i;
	for(i=0; i<networkTable.numberEntries[NEIGHBOR_ENTRY]; i++){
		if(id == network[i].neighbor.shNodeID){
			*index = i;
			return 1;
		}
	}
	return 0;
}

//returns 1 for success, 0 otherwise
uint8_t network_has_route(uint16_t id, uint8_t* index)
{
	uint8_t i;
	for(i=0; i<networkTable.numberEntries[ROUTING_ENTRY]; i++){
		if(id == network[MAX_NETWORK_ENTRIES - 1 - i].routing.mhNodeID){
			*index = MAX_NETWORK_ENTRIES - 1 - i;
			return 1;
		}
	}
	return 0;
}

uint8_t network_insert(union networkEntry* entry, enum networkEntryType type)
{
	uint8_t roomLeft = 0;
	roomLeft = network_room_check(type);
	if(roomLeft == 1){
		//printf("Room left for type %d\n", type);	
	//this clear currently isn't strictly needed since both structs inside the union have no padding bits...but this might not be the case forever. Also have to make sure there aren't compiler-specific differences w/ sizing the structs
	//*((uint32_t*)(&network[index])) = 0;		
		if(type == NEIGHBOR_ENTRY){
			network[networkTable.numberEntries[NEIGHBOR_ENTRY]] = *entry;
		}else{
			network[MAX_NETWORK_ENTRIES - 1 - networkTable.numberEntries[ROUTING_ENTRY]] = *entry;
		}
		networkTable.numberEntries[type]++;
		return 1;
	}else{
		//printf("No room left for type %d\n", type);	
		return -1;
	}
}

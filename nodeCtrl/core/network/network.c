#include <stdio.h>
#include "network.h"

union networkEntry* network;
struct networkControl* networkInfo;

void network_init(uint8_t division)
{
	network = ( (union networkEntry*)(&(memoryMap[MM_NETWORK_BASE/4].u8[MM_NETWORK_BASE%4])));
   networkInfo = (struct networkControl*)(&(memoryMap[MM_DEVICE_BASE/4].u8[MM_DEVICE_BASE%4]));

	networkInfo->numberEntries[NEIGHBOR_ENTRY] = 0;
	networkInfo->numberEntries[ROUTING_ENTRY] = 0;
	networkInfo->divisionIndex = division;
   networkInfo->networkConfig = 0;
}

uint8_t network_room_check(enum networkEntryType type)
{
	//TODO could combine the first two ifs
	if( (type == NEIGHBOR_ENTRY) && (networkInfo->numberEntries[NEIGHBOR_ENTRY] < networkInfo->divisionIndex) ){
		return 1; //room for another neighbor entry
	}else if( (type == ROUTING_ENTRY) && ( (MAX_NETWORK_ENTRIES-1) - networkInfo->numberEntries[ROUTING_ENTRY] >= networkInfo->divisionIndex) ){
		return 1; //room for another routing entry
	}else{
		return 0;
	}
}
//returns 1 for success, 0 otherwise
uint8_t network_has_neighbor(uint16_t id, uint8_t* index)
{
	uint8_t i;
	for(i=0; i<networkInfo->numberEntries[NEIGHBOR_ENTRY]; i++){
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
	for(i=0; i<networkInfo->numberEntries[ROUTING_ENTRY]; i++){
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
			network[networkInfo->numberEntries[NEIGHBOR_ENTRY]] = *entry;
		}else{
			network[MAX_NETWORK_ENTRIES - 1 - networkInfo->numberEntries[ROUTING_ENTRY]] = *entry;
		}
		networkInfo->numberEntries[type]++;
		return 1;
	}else{
		//printf("No room left for type %d\n", type);	
		return 0;
	}
}

uint8_t network_update(uint16_t id, uint8_t LQE, uint8_t radioId, uint8_t networkId, enum networkEntryType type)
{
  uint8_t index = 0;
  union networkEntry entry;
  uint8_t returnData = 0;
  if (type == NEIGHBOR_ENTRY){
    //check if this is a new neighbor
    if (network_has_neighbor(id, &index)){
      network[index].neighbor.shLQE = 0xFF & LQE;
#ifdef MSP430
      print_string("Neighbor: ", NONE); print_decimal(id, NEWLINE);
      print_string(", RSSI 0x", NONE); print_hex(network[index].neighbor.shLQE, NEWLINE);
#endif
    }else{ //make a new entry
      entry.neighbor.shNodeID = id;
      entry.neighbor.shLQE = 0xFF & LQE;
      entry.neighbor.radioID = radioId;
      entry.neighbor.networkID = networkId;
      returnData = network_insert(&entry, NEIGHBOR_ENTRY); 
#ifdef MSP430
      if (returnData == 1){
        print_string("Added neighbor: ", NONE); 
        print_hex(id, NEWLINE);
      }
      else{
        print_string("No room for neighbor: ", NONE); 
        print_decimal(id, NEWLINE);
      }
#endif
    }
  }
  return returnData;
//FIXME: UNUSED
#if 0
  else if (type == ROUTING_ENTRY)
  {
    //check if this is a new route
    if (network_has_route(id, &index)){
      network[index].routing.mhLQE = 0xFF & LQE;
#ifdef MSP430
      Serial.print("Route: "); Serial.print(rxRawPacket.hdr.src); 
      Serial.print(", RSSI 0x"); Serial.println(network[index].routing.mhLQE, HEX);
#endif
    }else{
      entry.routing.mhNodeID = id;
      entry.routing.hLQE = 0xFF & LQE;
      entry.routing.radioID = radioId;
      entry.routing.networkID = networkId;
      returnData = network_insert(&entry, ROUTING_ENTRY); 
#ifdef MSP430
      if (returnData == 1){
        Serial.print("Added route: "); 
        Serial.println(id);
      }
      else{
        Serial.print("No room for route to: "); 
        Serial.println(id);
      }
#endif
    }
  }
#endif
}

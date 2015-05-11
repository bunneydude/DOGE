#include <stdio.h>
#include <string.h>
#include "network.h"
#include "../protocol/type.h"
#include "../platform/platform.h"

union networkEntry* network;
struct networkControl* networkInfo;

void network_init(uint8_t division)
{
   network = ( (union networkEntry*)(&(memoryMap[MM_NETWORK_BASE/4].u8[MM_NETWORK_BASE%4])));
   networkInfo = (struct networkControl*)(&(memoryMap[MM_DEVICE_BASE/4].u8[MM_DEVICE_BASE%4]));

   networkInfo->numberEntries[NEIGHBOR_ENTRY] = 0;
   networkInfo->numberEntries[ROUTING_ENTRY] = 0;
   networkInfo->divisionIndex = division;
   networkInfo->networkConfig = LQE_UPDATE;
   memset(network, 0, MAX_NETWORK_ENTRIES*sizeof(union networkEntry));
}

dogeBool network_room_check(enum networkEntryType type)
{
   //TODO could combine the first two ifs
   if( (type == NEIGHBOR_ENTRY) && (networkInfo->numberEntries[NEIGHBOR_ENTRY] < networkInfo->divisionIndex) ){
      return TRUE; //room for another neighbor entry
   }else if( (type == ROUTING_ENTRY) && ( (MAX_NETWORK_ENTRIES-1) - networkInfo->numberEntries[ROUTING_ENTRY] >= networkInfo->divisionIndex) ){
      return TRUE; //room for another routing entry
   }else{
      return FALSE;
   }
}

/** 
 * @brief Searches the neighbor table for the requested node ID.
 * @param id Node ID to search.
 * @param index Index in the network table if Node ID is found.
 * @param radioID Radio ID to search.
 * @param includeMasked Include hidden neighbors in the search.
 * @return TRUE for success, FALSE otherwise
 */
dogeBool network_has_neighbor(uint16_t id, uint8_t* index, uint8_t radioID, dogeBool includeMasked)
{
   uint8_t i = 0;
   int8_t match = -1;
   for (i = 0; i < networkInfo->numberEntries[NEIGHBOR_ENTRY]; i++){
      if ((id == network[i].neighbor.shNodeID) &&
          (radioID == RADIO_ID_ALL || network[i].neighbor.radioID == radioID) &&
          (includeMasked || network[i].neighbor.shLQE != MASKED_LQE)){
         if (match == -1 || (network[i].neighbor.shLQE > network[match].neighbor.shLQE)){
            match = i;
         }
      }
   }
   if (match >= 0){
      *index = match;
      return TRUE;
   }
   else{
      return FALSE;
   }
}

/** 
 * @brief Searches the routing table for a route to the destination node.
 * @param id Destination Node ID to search.
 * @param index Index in the network table if Node ID is found.
 * @param radioID Radio ID to search.
 * @param includeMasked Include hidden routes in the search.
 * @return TRUE for success, FALSE otherwise
 */
dogeBool network_has_route(uint16_t id, uint8_t* index, uint8_t radioID, dogeBool includeMasked)
{
   uint8_t i = 0;
   uint8_t routeIndex = 0;
   uint8_t currNeighborIndex = 0;
   uint8_t prevNeighborIndex = 0;
   int8_t match = -1;
   for (i = 0; i < networkInfo->numberEntries[ROUTING_ENTRY]; i++){
      routeIndex = MAX_NETWORK_ENTRIES - 1 - i;
      if (id == network[routeIndex].routing.mhNodeID){
         currNeighborIndex = network[routeIndex].routing.neighborIndex;
         if((network[currNeighborIndex].neighbor.shLQE == MASKED_LQE && !includeMasked) ||
            (network[currNeighborIndex].neighbor.radioID != radioID && radioID != RADIO_ID_ALL)){
            continue;
         }
         if (match == -1){
            match = routeIndex;
         }
         else{
            prevNeighborIndex = network[match].routing.neighborIndex;
            if(network[currNeighborIndex].neighbor.shLQE > network[prevNeighborIndex].neighbor.shLQE){
               match = routeIndex;
            }
         }
      }
   }
   if (match >= 0){
      *index = match;
      return TRUE;
   }
   else{
      return FALSE;
   }
}

dogeBool network_insert(union networkEntry* entry, enum networkEntryType type)
{
   uint8_t roomLeft = FALSE;
   roomLeft = network_room_check(type);
   /* Return immediately if the table is full */
   if(roomLeft == FALSE){
     return FALSE;
   }
   /* Validate routes */
   if (type == ROUTING_ENTRY){
     if (entry->routing.neighborIndex >= networkInfo->numberEntries[NEIGHBOR_ENTRY] || 
         /* Do not add routes to a single hop neighbor through that same neighbor */
         network[entry->routing.neighborIndex].neighbor.shNodeID == entry->routing.mhNodeID){
       return FALSE;
     }
   }
   //printf("Room left for type %d\n", type);
   //this clear currently isn't strictly needed since both structs inside the union have no padding bits...but this might not be the case forever. Also have to make sure there aren't compiler-specific differences w/ sizing the structs
   //*((uint32_t*)(&network[index])) = 0;
   if(type == NEIGHBOR_ENTRY){
     network[networkInfo->numberEntries[NEIGHBOR_ENTRY]] = *entry;
   }else{
     network[MAX_NETWORK_ENTRIES - 1 - networkInfo->numberEntries[ROUTING_ENTRY]] = *entry;
   }
   networkInfo->numberEntries[type]++;
   return TRUE;
}

uint8_t network_update(uint16_t id, uint8_t LQE, uint8_t radioID, uint8_t networkID, enum networkEntryType type)
{
   uint8_t index = 0;
   union networkEntry entry;
   uint8_t returnData = 0;
   if (type == NEIGHBOR_ENTRY){
      //check if this is a new neighbor
      if (network_has_neighbor(id, &index, radioID, FALSE)){
         if ((networkInfo->networkConfig) & LQE_UPDATE){
            if (LQE == PERFECT_LQE){
               LQE = MAX_LQE;
            }
            else if (LQE == MASKED_LQE){
               LQE = MIN_LQE;
            }
            network[index].neighbor.shLQE = 0xFF & LQE;
         }
         print_string("Neighbor: ", NONE); print_decimal(id, NEWLINE);
         print_string(", RSSI 0x", NONE); print_hex(network[index].neighbor.shLQE, NEWLINE);
      }else{ //make a new entry
         entry.neighbor.shNodeID = id;
         entry.neighbor.shLQE = 0xFF & LQE;
         entry.neighbor.radioID = radioID;
         entry.neighbor.networkID = networkID;
         returnData = network_insert(&entry, NEIGHBOR_ENTRY); 
         if (returnData == 1){
            print_string("Added neighbor: ", NONE); 
            print_hex(id, NEWLINE);
         }
         else{
            print_string("No room for neighbor: ", NONE); 
            print_decimal(id, NEWLINE);
         }
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

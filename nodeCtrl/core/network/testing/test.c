#include <stdlib.h>
#include <stdio.h>
#include "network.h"
#include "../platform/platform.h"

extern union networkEntry* network;
extern struct networkControl* networkInfo;

#ifdef LINUX
#define TEST_SH_LOW_LQE    (0x10)
#define TEST_SH_MEDIUM_LQE (0x7F)
#define TEST_SH_HIGH_LQE   (0xE7)

void print_entry(union networkEntry e, enum networkEntryType type);

void print_contents(){
   uint8_t i = 0;
   uint8_t j = 0;
   uint8_t x;
   printf("\n");
   for (i = 0; i < networkInfo->numberEntries[NEIGHBOR_ENTRY]; i++){
      print_entry(network[i], NEIGHBOR_ENTRY);
   }
   for (i = MAX_NETWORK_ENTRIES - 1; i > MAX_NETWORK_ENTRIES - 1 - networkInfo->numberEntries[ROUTING_ENTRY];  i--){
      print_entry(network[i], ROUTING_ENTRY);
   }
}
void print_entry(union networkEntry e, enum networkEntryType type)
{
   printf("Number neighbor entries = %d, number routing entries = %d, division index = %d\n", networkInfo->numberEntries[NEIGHBOR_ENTRY], networkInfo->numberEntries[ROUTING_ENTRY], networkInfo->divisionIndex);

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

uint8_t i = 0;

void init_test_table_1()
{
   //Neighbor Table
   insert_neighbor(NODE_ID_2, PERFECT_LQE, RADIO_ID_915);
   insert_neighbor(NODE_ID_4, MASKED_LQE, RADIO_ID_915);
   insert_neighbor(NODE_ID_5, PERFECT_LQE, RADIO_ID_915);
   insert_neighbor(NODE_ID_6, MASKED_LQE, RADIO_ID_915);
   //Routing Table
   insert_route(ROOT_NODE, NODE_ID_2, MASKED_LQE, RADIO_ID_ALL);
   insert_route(NODE_ID_4, NODE_ID_5, MASKED_LQE, RADIO_ID_ALL);
   insert_route(NODE_ID_7, NODE_ID_5, MASKED_LQE, RADIO_ID_ALL);
   insert_route(NODE_ID_7, NODE_ID_4, MASKED_LQE, RADIO_ID_ALL);
}

void init_test_table_2()
{
   /*
   * neighbor = {shNodeID = 0x4, shLQE = 0x10, radioID = 0x1, networkID = 0x0},
   * neighbor = {shNodeID = 0x4, shLQE = 0x0, radioID = 0x2, networkID = 0x0},
   * neighbor = {shNodeID = 0x4, shLQE = 0x7f, radioID = 0x1, networkID = 0x1},
   * neighbor = {shNodeID = 0x4, shLQE = 0xe7, radioID = 0x2, networkID = 0x1},
   * routing = {mhNodeID = 0x7, mhLQE = 0x0, neighborIndex = 0x3},
   * routing = {mhNodeID = 0x7, mhLQE = 0x0, neighborIndex = 0x2},
   * routing = {mhNodeID = 0x7, mhLQE = 0x0, neighborIndex = 0x1},
   * routing = {mhNodeID = 0x7, mhLQE = 0x0, neighborIndex = 0x0}}
   */
   int8_t index;
   network_init(NETWORK_DIVISION_DEFAULT);
   //Insert first two neighbors
   insert_neighbor(NODE_ID_4, TEST_SH_LOW_LQE, RADIO_ID_915);
   insert_neighbor(NODE_ID_4, MASKED_LQE, RADIO_ID_2400);
   
   //Insert the other neighbors gradually to get different routes
   network_has_neighbor(NODE_ID_4, &index, RADIO_ID_915, TRUE);
   struct routingEntry routingEntry1 = {
      NODE_ID_7, 0x0, index };

   network_has_neighbor(NODE_ID_4, &index, RADIO_ID_2400, TRUE);
   struct routingEntry routingEntry2 = {
      NODE_ID_7, 0x0, index };
   insert_neighbor(NODE_ID_4, TEST_SH_MEDIUM_LQE, RADIO_ID_915);

   network_has_neighbor(NODE_ID_4, &index, RADIO_ID_915, TRUE);
   struct routingEntry routingEntry3 = {
      NODE_ID_7, 0x0, index };
   insert_neighbor(NODE_ID_4, TEST_SH_HIGH_LQE, RADIO_ID_2400);

   network_has_neighbor(NODE_ID_4, &index, RADIO_ID_2400, TRUE);
   struct routingEntry routingEntry4 = {
      NODE_ID_7, 0x0, index };

   network_insert((union networkEntry*)&routingEntry1, ROUTING_ENTRY);
   network_insert((union networkEntry*)&routingEntry2, ROUTING_ENTRY);
   network_insert((union networkEntry*)&routingEntry3, ROUTING_ENTRY);
   network_insert((union networkEntry*)&routingEntry4, ROUTING_ENTRY);
}

void main(){
   //init network
   uint8_t index;

   network_init(NETWORK_DIVISION_DEFAULT);
   init_test_table_1();

#ifdef LINUX
   printf("N size = 0x%lu, R size = 0x%lu\n", sizeof(struct neighborEntry), sizeof(struct routingEntry));
   printf("N entries = %d, R entries = %d\n", networkInfo->numberEntries[NEIGHBOR_ENTRY], networkInfo->numberEntries[ROUTING_ENTRY]);
   printf("Entry division index = %d, max network entries = %d\n", networkInfo->divisionIndex, MAX_NETWORK_ENTRIES);
#endif

   network[0].neighbor.shLQE = 1;
   print_entry(network[0], NEIGHBOR_ENTRY);

   print_contents();

   printf("Orig LQE = %x\n", network[0].neighbor.shLQE);
   if(network_has_neighbor(NODE_ID_2, &index, RADIO_ID_ALL, FALSE)){
      network[index].neighbor.shLQE = 0x5;
   }
   printf("New LQE = %x\n", network[0].neighbor.shLQE);

   printf("Orig LQE = %d\n", network[MAX_NETWORK_ENTRIES-1].routing.mhLQE);
   if(network_has_route(ROOT_NODE, &index, RADIO_ID_ALL, FALSE)){
      network[index].routing.mhLQE = 0x6;
   }
   printf("New LQE = %d\n", network[MAX_NETWORK_ENTRIES-1].routing.mhLQE);

   /* Test 1 - Make sure checking for a masked neighbor returns FALSE */
   if (network_has_neighbor(NODE_ID_4, &index, RADIO_ID_ALL, FALSE)){
      printf("Test 1 FAILED. NODE_ID_4 is supposed to be masked\n");
   }
   else{
      printf("Test 1 PASSED. NODE_ID_4 is masked\n");
   }

   /* Test 2 - Make sure checking for a masked neighbor with includeMasked == TRUE returns TRUE */
   if (network_has_neighbor(NODE_ID_4, &index, RADIO_ID_ALL, TRUE)){
      printf("Test 2 PASSED. NODE_ID_4 is masked, and we detected it.\n");
   }
   else{
      printf("Test 2 FAILED. NODE_ID_4 is masked, but we should be able to detect it.\n");
   }

   init_test_table_2();
   
   /* Test 3 - Highest LQE entry is selected from the neighbor table */
   if (network_has_neighbor(NODE_ID_4, &index, RADIO_ID_ALL, FALSE) && index == 0x3){
      printf("Test 3 PASSED. Selected last entry in the neighbor table with the highest LQE. \n");
   }
   else{
      printf("Test 3 FAILED. Did not select highest LQE entry.\n");
   }
   
   /* Test 4 - Highest LQE entry is selected from the routing table */
   if (network_has_route(NODE_ID_7, &index, RADIO_ID_ALL, FALSE) && index == 0x4){
      printf("Test 4 PASSED. Selected last entry in the routing table with the highest LQE. \n");
   }
   else{
      printf("Test 4 FAILED. Did not select highest LQE entry.\n");
   }

   /* Test 5 - Network update does not set masked LQEs. */
   network_update(NODE_ID_4, MASKED_LQE, RADIO_ID_915, NETWORK_ID_0, NEIGHBOR_ENTRY);
   if(network[2].neighbor.shLQE != MIN_LQE){
      printf("Test 5 FAILED. Network update set a reserved LQE. \n");
   }
   else{
      printf("Test 5 PASSED. Network update set MIN LQE for a reserved masked LQE. \n");
   }
   
   /* Test 6 - Network update does not set perfect LQEs. */
   network_update(NODE_ID_4, PERFECT_LQE, RADIO_ID_915, NETWORK_ID_0, NEIGHBOR_ENTRY);
   if(network[0].neighbor.shLQE != MAX_LQE){
      printf("Test 6 FAILED. Network update set a reserved LQE. \n");
   }
   else{
      printf("Test 6 PASSED. Network update set MAX LQE for a reserver perfect LQE. \n");
   }

   /* Test 7 - Network has neighbor returns FALSE for a neighbor on a specific frequency. */
   if (!network_has_neighbor(NODE_ID_4, &index, RADIO_ID_433, FALSE)){
      printf("Test 7 PASSED. Network does not have a neighbor on this frequency. \n");
   }
   else{
      printf("Test 7 FAILED. Network should not have a neighbor on this frequency. \n");
   }
   
   /* Test 8 - Network has route returns FALSE for a route on a specific frequency. */
   if (!network_has_neighbor(NODE_ID_7, &index, RADIO_ID_433, FALSE)){
      printf("Test 8 PASSED. Network does not have a route on this frequency. \n");
   }
   else{
      printf("Test 8 FAILED. Network should not have a route on this frequency. \n");
   }
}

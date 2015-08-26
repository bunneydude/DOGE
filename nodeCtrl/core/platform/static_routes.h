#ifndef STATIC_ROUTES_H
#define STATIC_ROUTES_H
#include <stdint.h>
#include "../memory_map/memory_map.h"
#include "../nodeCtrl_errno.h"
#include "../packet.h"
#include "../protocol/protocol.h"
#include "type.h"
#include "../platform/platform.h"
#include "../nrfLegacy/nrfLegacy.h"
#include "../network/network.h"

#define NRF24_DEFAULT_MM_MASK (0xFF)
#define NRF24_DEFAULT_APP_ADDRESS (0x00)
#define NRF24_STARTING_NODE_ID (11)
#define NRF24_NODE_OFFSET_TO_ID(OFFSET) ((OFFSET) + (NRF24_STARTING_NODE_ID))

typedef enum{
   NRF24_NODE_11_OFFSET = 0,
   NRF24_NODE_12_OFFSET = 1,
   NRF24_NODE_13_OFFSET = 2,
   NRF24_NODE_14_OFFSET = 3,
   NRF24_NODE_15_OFFSET = 4,
   NRF24_NODE_16_OFFSET = 5,
   NRF24_NODE_17_OFFSET = 6,
   NRF24_NODE_18_OFFSET = 7
}nrf24NodeOffsets;

typedef enum{
   NRF24_NODE_11 = 11,
   NRF24_NODE_12 = 12,
   NRF24_NODE_13 = 13,
   NRF24_NODE_14 = 14,
   NRF24_NODE_15 = 15,
   NRF24_NODE_16 = 16,
   NRF24_NODE_17 = 17,
   NRF24_NODE_18 = 18
}nrf24NodeIDs;

typedef enum{
   NRF24_NODE_11_STATIC_ADDRESS = MM_STATIC_ROUTE_BASE + NRF24_NODE_11_OFFSET,
   NRF24_NODE_12_STATIC_ADDRESS = MM_STATIC_ROUTE_BASE + NRF24_NODE_12_OFFSET,
   NRF24_NODE_13_STATIC_ADDRESS = MM_STATIC_ROUTE_BASE + NRF24_NODE_13_OFFSET,
   NRF24_NODE_14_STATIC_ADDRESS = MM_STATIC_ROUTE_BASE + NRF24_NODE_14_OFFSET,
   NRF24_NODE_15_STATIC_ADDRESS = MM_STATIC_ROUTE_BASE + NRF24_NODE_15_OFFSET,
   NRF24_NODE_16_STATIC_ADDRESS = MM_STATIC_ROUTE_BASE + NRF24_NODE_16_OFFSET,
   NRF24_NODE_17_STATIC_ADDRESS = MM_STATIC_ROUTE_BASE + NRF24_NODE_17_OFFSET,
   NRF24_NODE_18_STATIC_ADDRESS = MM_STATIC_ROUTE_BASE + NRF24_NODE_18_OFFSET
}nrf24NodeAddresses;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void insert_neighbor(uint16_t nodeID, uint16_t LQE, uint8_t radioID);
void insert_route(uint16_t dstNodeID, uint16_t neighborID, uint16_t LQE, uint8_t radioID);
void setup_iotg_demo_grid();
uint8_t static_route_mm_handler(uint8_t rw, uint8_t addr, uint8_t* data, uint8_t mask);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

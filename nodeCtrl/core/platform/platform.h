#ifndef PLATFORM_H
#define PLATFORM_H

// -----------------------------------------------------------------------------
/**
 *  Global data
 */
/**@brief NodeCtrl version */
#define NODECTRL_VERSION 1

/**@brief Node ID 1 **/
#define NODE_ID_1 0x1
/**@brief Node ID 2 **/
#define NODE_ID_2 0x2
/**@brief Node ID 3 **/
#define NODE_ID_3 0x3
/**@brief Node ID 4 **/
#define NODE_ID_4 0x4
/**@brief Node ID 5 **/
#define NODE_ID_5 0x5
/**@brief Node ID 6 **/
#define NODE_ID_6 0x6
/**@brief Node ID 7 **/
#define NODE_ID_7 0x7
/**@brief Node ID 8 **/
#define NODE_ID_8 0x8
/**@brief Node ID 9 **/
#define NODE_ID_9 0x9
/**@brief Node ID 10 **/
#define NODE_ID_10 0xa
/**@brief Node ID 11 **/
#define NODE_ID_11 0xb
/**@brief Node ID 12 **/
#define NODE_ID_12 0xc
/**@brief Root Node ID(Edison) **/
#define ROOT_NODE NODE_ID_1

/**@brief Maximum Radio Packet Length */
#define MAX_DATA_LENGTH 32

/**@brief 915MHz Radio ID */
#define RADIO_ID_915 (0x1)
/**@brief Network ID */
#define NETWORK_ID_0 (0x0)
// -----------------------------------------------------------------------------

#define MY_NODE_ID NODE_ID_3

#ifdef MSP430
#include "doge_timers.h"
#include "reliable_channel.h"
#endif
#include "serial_c.h"
#endif

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
/**@brief Root Node ID 2 **/
#define NODE_ID_2 0x2
/**@brief Root Node ID 3 **/
#define NODE_ID_3 0x3
/**@brief Root Node ID 4 **/
#define NODE_ID_4 0x4
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

#include "doge_timers.h"
#include "reliable_channel.h"
#include "serial_c.h"
#endif

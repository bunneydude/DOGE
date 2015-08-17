#ifndef PLATFORM_H
#define PLATFORM_H

// -----------------------------------------------------------------------------
/**
 *  Global data
 */
/**@brief NodeCtrl version */
#define NODECTRL_VERSION 1

#ifdef __cplusplus
#define __STDC_LIMIT_MACROS
#endif

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
/**@brief Node ID 13 **/
#define NODE_ID_13 0xd
/**@brief Root Node ID(Edison) **/
#define ROOT_NODE NODE_ID_1

/**@brief Maximum Radio Packet Length */
#define MAX_DATA_LENGTH 32

/**@brief 433MHz Radio ID */
#define RADIO_ID_433 (0x0)
/**@brief 915MHz Radio ID */
#define RADIO_ID_915 (0x1)
/**@brief 2.4GHz Radio ID */
#define RADIO_ID_2400 (0x2)
/**@brief Radio ID for all frequency bands */
#define RADIO_ID_ALL (0x4)
/**@brief Network ID 0 */
#define NETWORK_ID_0  (0x0)
/**@brief Network ID 1 */
#define NETWORK_ID_1  (0x1)
/**@brief Network ID 2 */
#define NETWORK_ID_2  (0x2)

// -----------------------------------------------------------------------------

#define DEMO_GRID 1
#define MY_NODE_ID NODE_ID_2
#define DBG 0

#ifdef __LPC8XX__
#define TEMP_SENSOR   0
#define LED_LOCATION  2
#define RED_LED       LED_LOCATION
#define RADIO_NRF_CE  14
#define RADIO_NRF_CSN 7
#define NRF_IRQ       17
#define LPC_ISP       12
#define LPC_SCL       11
#define LPC_SDA       10
#define LPC_ADC       6
#define LPC_TEMP1     9
#define LPC_TEMP2     1
#define LPC_TP1       16
#define LPC_TP2       15
#define RED_CH        0
#define GREEN_CH      1
#define BLUE_CH       2
#define ADDRESS_BROADCAST 0
#elif defined(MSP430)
//#define DUAL_RADIO
#define TEMP_SENSOR   A3
#define RADIO_NRF_CE  P2_3
#define RADIO_NRF_CSN P2_1
#endif

#include "doge_timers.h"
#include "doge_gpio.h"
#include "doge_radio.h"
#include "doge_adc.h"
#include "static_routes.h"
#include "serial_c.h"

#ifdef __LPC8XX__
#include "reliable_channel.h"
/* Undefine nrf24.h CONFIG definition */
#undef CONFIG
#include <LPC8XX.h>
#include "lpc812/gpio/gpio.h"
#include "lpc812/initializer.h"
#include "lpc812/uart.h"
#include "lpc812/spi.h"
#include "lpc812/pwm.h"
#elif defined(MSP430) && defined(__cplusplus)
#include "reliable_channel.h"
#endif
#endif

#ifndef DOGE_RADIO_H
#define DOGE_RADIO_H
#include <stdint.h>

#ifdef __cplusplus
#include <SPI.h>
#include <AIR430BoostFCC.h>
#endif

#ifndef __cplusplus
/*
* Define Air430BoostFCC enums for C code
* See:
* energia-0101E0015/hardware/msp430/libraries/AIR430BoostUSAFCC/utility/A110x2500Radio.h
*/
/**
 *  eChannel - frequency (channel).
 *
 *  Note: These values meet regulatory compliance with the provided 
 *  configuration for both ETSI and FCC/IC.
 */
typedef enum eChannel
{
  CHANNEL_1  = 0x05,  // ETSI 868.3MHz; FCC/IC 903MHz
  CHANNEL_2  = 0x0F,  // ETSI 868.8MHz; FCC/IC 904MHz
    CHANNEL_3  = 0x19,  // ETSI 869.3MHz; FCC/IC 905MHz
  CHANNEL_4  = 0x23,  // ETSI 869.8MHz; FCC/IC 906MHz
} channel_t;

/**
 *  ePower - power table values as indexes into the compiled power lookup table.
 *
 *  Note: These values meet regulatory compliance with the provided 
 *  configuration for both ETSI and FCC/IC.
 */
typedef enum ePower
{
  POWER_4_DBM  = 0,   // 4dBm
  POWER_3_DBM  = 1,   // 3dBm
  POWER_2_DBM  = 2,   // 2dBm
  POWER_1_DBM  = 3,   // 1dBm
  POWER_0_DBM  = 4    // 0dBm
} power_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

unsigned char radio_receiver_on(uint8_t *dataField, uint8_t length, uint16_t timeout);
void radio_begin(uint8_t address, channel_t channel, power_t power);
void radio_transmit(uint8_t address, uint8_t *dataField, uint8_t length);

#ifdef __cplusplus
}
#endif
#endif

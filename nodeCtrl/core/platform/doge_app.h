#ifndef DOGE_APP_H
#define DOGE_APP_H
#include <platform.h>
#ifdef __LPC8XX__
#include "lpc812/pwm.h"
#include "lpc812/printf.h"
#endif
#include <protocol.h>

#ifdef LINUX
#define NO_RESPONSE (0x04)
#include <stdio.h>
#endif

#if defined(__LPC8XX__) || defined(LINUX)
#define feed 0x80000057
typedef uint32_t lfsrType;
#elif defined(MSP43)
#define feed 0x8016
typedef uint16_t lfsrType;
#endif

#define INIT (0x1)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
lfsrType rand_int();
void user_application_setup();
void user_application_loop();
uint8_t user_application_form_packet(userAppPacket* packet, packetAttr* attr, uint8_t cmd, uint8_t size, uint8_t* bytes);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

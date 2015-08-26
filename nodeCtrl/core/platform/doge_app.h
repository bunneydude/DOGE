#ifndef DOGE_APP_H
#define DOGE_APP_H
#include <platform.h>
#ifdef __LPC8XX__
#include "lpc812/pwm.h"
#include "lpc812/printf.h"
#endif
#include <protocol.h>

#ifdef LINUX
#define NO_RESPONSE (0x01010101)
#define RESPONSE (0x02020202)
#define SAMPLE_DATA (0x89abcdef)
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void user_application_setup();
void user_application_loop();
uint8_t user_application_form_packet(userAppPacket* packet, packetAttr* attr, uint8_t cmd, uint8_t size, uint8_t* bytes);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

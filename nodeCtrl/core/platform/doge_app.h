#ifndef DOGE_APP_H
#define DOGE_APP_H
#include <platform.h>
#ifdef __LPC8XX__
#include "lpc812/pwm.h"
#include "lpc812/printf.h"
#endif

void user_application_setup();
void user_application_loop();

#endif

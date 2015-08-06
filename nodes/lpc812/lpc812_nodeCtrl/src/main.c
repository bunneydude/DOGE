/**************************************************************************/
/*!
    @file     main.c

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, K. Townsend (microBuilder.eu)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**************************************************************************/
#include <stdio.h>
#include "LPC8xx.h"
#include "main.h"
#include "mrt.h"
#include "lpc_type.h"
#include "radios/radios.h"
#include "nodeCtrl.h"
#include "platform/lpc812/uart.h"
#include "platform/lpc812/gpio/gpio.h"
#include "platform/platform.h"


#include <cr_section_macros.h>

void char_to_RGB(uint8_t input, uint8_t* red, uint8_t* green, uint8_t* blue){

   if(input < 20){
      //R = [255, 0]
      //G = 0
      //B = 255
      *red = ( ( (uint32_t)(20 - input) )/20.0)*100;
      *green = 0;
      *blue = 100;

   }else if( (20 <= input) && (input < 40)){
      //R = 0
      //G = [0,255]
      //B = 255

      *red = 0;
      *green = ( ( (uint32_t)(input - 20) )/20.0)*100;
      *blue = 100;

   }else if( (40 <= input) && (input < 60)){
      //R = 0
      //G = 255
      //B = [255, 0]

      *red = 0;
      *green = 100;
      *blue = ( ( (uint32_t)(60 - input) )/20.0)*100;

   }else if( (60 <= input) && (input < 80)){
      //R = [0, 255]
      //G = 255
      //B = 0

      *red = ( ( (uint32_t)(input - 60) )/20.0)*100;
      *green = 100;
      *blue = 0;

   }else{
      //R = 255
      //G = [255, 0]
      //B = 0

      *red = 100;
      *green = ( ( (uint32_t)(100 - input) )/20.0)*100;
      *blue = 0;

   }
}

int main(void)
{
   nodeCtrl_init();
   nodeCtrl_entry();
}

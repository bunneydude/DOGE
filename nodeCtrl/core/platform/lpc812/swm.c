/*
* @brief
* This file is used to configure the  SwitchMatrix module.
*
* @note
* Copyright(C) NXP Semiconductors, 2015
* All rights reserved.
*
* @par
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* LPC products.  This software is supplied "AS IS" without any warranties of
* any kind, and NXP Semiconductors and its licensor disclaim any and
* all warranties, express or implied, including all implied warranties of
* merchantability, fitness for a particular purpose and non-infringement of
* intellectual property rights.  NXP Semiconductors assumes no responsibility
* or liability for the use of the software, conveys no license or rights under any
* patent, copyright, mask work right, or any other intellectual property rights in
* or to any products. NXP Semiconductors reserves the right to make changes
* in the software without notification. NXP Semiconductors also makes no
* representation or warranty that such application will be suitable for the
* specified use without further testing or modification.
*
* @par
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors' and its
* licensor's relevant copyrights in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
*/

#include "LPC8xx.h"    /* CMSIS  Peripheral Registers for LPC812M101FD20 */

void SwitchMatrix_Init()
{
   /* Enable the clock to the Switch Matrix */
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);

   /* Pin Assign 8 bit Configuration */
   /* U0_TXD */
   /* U0_RXD */
   LPC_SWM->PINASSIGN0 = 0xffff0004UL;
   /* SPI0_SCK */
   LPC_SWM->PINASSIGN3 = 0x08ffffffUL;
   /* SPI0_MOSI */
   /* SPI0_MISO */
   LPC_SWM->PINASSIGN4 = 0xffff0d03UL;
   /* SPI0_SSEL */
   //LPC_SWM->PINASSIGN4 = 0xff070d03UL;

   /* Pin Assign 1 bit Configuration */
   /* RESET */
   LPC_SWM->PINENABLE0 = 0xffffffbfUL;
}

/**********************************************************************
**                            End Of File
**********************************************************************/

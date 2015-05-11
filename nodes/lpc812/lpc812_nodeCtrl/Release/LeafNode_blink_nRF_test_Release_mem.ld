/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-2015
 * (c) NXP Semiconductors 2013-2015
 * Linker script memory definitions
 * Created from LinkMemoryTemplate
 * By LPCXpresso v7.4.0 [Build 229] [2014-09-16]  on Mon May 11 00:12:18 PDT 2015)
*/

MEMORY
{
  /* Define each memory region */
  MFlash16 (rx) : ORIGIN = 0x0, LENGTH = 0x4000 /* 16K bytes */
  RamLoc4 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x1000 /* 4K bytes */


}
  /* Define a symbol for the top of each memory region */
  __top_MFlash16 = 0x0 + 0x4000;
  __top_RamLoc4 = 0x10000000 + 0x1000;




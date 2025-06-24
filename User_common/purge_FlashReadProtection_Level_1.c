#include "stm32f4xx.h"

//NOTE!!!  Read Protection Level 2 is irreversible, so it can never be changed,
//          and flash Rom is then NEVER reprogrammable!!!!!!!!

//============================================
// RDP value in Option Bytes (at 0x1FFF C000~0x1FFF C00F)
//   can be changed from Level 1 (RDP= any value other than AA and CC) 
//   to Level 0 (RDP = AA) by booting code from RAM to call the following
//   Function, which set FLASH->OPTCR.RDP = AA
//---------------------------------------------------------
//@par How to use it ? 
//
//In order to make the program work, you must do the following :
// - 1. Open "Options for Target" subwindow in Keil MDK
// -   then select "Debug" page; key in ".\Dbg_RAM.ini" in "Initialization File" blank 
// - 2. select "Target" page, and fill 0x2000 0000 and 0x1 0000 in 
// -     "Start and "Size" of "IROM1" item, and then
// -     fill 0x2001 0000 and 0x1 0000 in "Start and "Size" of "IRAM1" item  
// - Rebuild all files and press "Debug Section" button in MDK
// - The file "\Dbg_RAM.ini" will automatically load the code in the memory insted of the flash
// - Run the code and watch register "FLASH->OPTCR"
// - Leave the debug Section
//---------------------------------------------
	uint32_t OptionByte, WriteOption;

void PURGE_FLASH_READ_PROTECTION(void)
{
      FLASH->OPTKEYR = ((uint32_t)0x08192A3B);    // OPTKEY 1
      FLASH->OPTKEYR = ((uint32_t)0x4C5D6E7F);    // OPTKEY 2
	while((FLASH->SR & 0x00010000 ) != 0)
		FLASH->OPTCR = ((uint32_t)0x0FFFAAEC);    // bits 8~15: 0xAA ==> Level 0; bits 2~3: 11b ==> BOR off; bits 4~7: 0xE. 
	  FLASH->OPTCR = ((uint32_t)0x0FFFAAEE);      // bit 1: start; bits 18~21=section 2~5: 1= Write allowable
	
	while((FLASH->SR & 0x00010000 ) != 0);
	
	OptionByte= *(uint32_t*)0x1FFFC000;
	WriteOption= *(uint32_t*)0x1FFFC008;
}

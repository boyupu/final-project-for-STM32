/*----------------------------------------------------------------------------
 * Name:    STM32_nvic.c
 * Purpose: STM32 nested vector interrupt controller initialisation
  * Note(s):
 *----------------------------------------------------------------------------
 * This file was written by Shir-Kuan Lin (NCTU, Hsinchu, Taiwan)
 *----------------------------------------------------------------------------*/

//#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
//#include "STM32_Reg.h"                            // missing bit definitions
#include "stm32f10x.h"

#define Bit(x) ( (u32)1<<x )
//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//


#define __PRE_BITS	2


//=========================================================================== Clock Configuration
//=========================================================================== Clock Configuration
// <e0.8> IRQ 0-7 Channels: Non-system-handler (external/maskable) Interrupts
// <e0.0> 0: WWDG (window watchdog) Interrupt Priority 
//      <o1.0..3> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.0..3> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.0> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 0
//-------------------------------------------------------------
// <e0.1> 1: PVD (PVD through EXTI) Interrupt Priority 
//      <o1.4..7> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.4..7> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.1> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 1
//-------------------------------------------------------------
// <e0.2> 2: TAMP_STAMP Interrupt Priority
//      <o1.8..11> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.8..11> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
// </e> End of 2
//-------------------------------------------------------------
// <e0.3> 3: RTC_WKUP Interrupt Priority
//      <o1.12..15> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.12..15> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.3> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 3
//-------------------------------------------------------------
// <e0.4> 4: FLASH (flash global) Interrupt Priority 
//      <o1.16..19> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.16..19> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.4> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 4
//-------------------------------------------------------------
// <e0.5> 5: RCC (reset and clock control) Interrupt Priority
//      <o1.20..23> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.20..23> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.5> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 5
//-------------------------------------------------------------
// <e0.6> 6: EXTI0 (External Interrupt Line 0) Interrupt Priority
//      <o1.24..27> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o1.24..27> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.6> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 6
//-------------------------------------------------------------
// <e0.7> 7: EXTI1 (External Interrupt Line 1) Interrupt Priority
//      <o1.28..31> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.28..31> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.7> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 7
//-------------------------------------------------------------
// </e>
//
#define __SETUP_CHL0_7	0x0104
#define __PREP_CHL0_7	0x00000300
#define __SUBP_CHL0_7	0x00000000
#define __ENABLE_CHL0_7	0x00

//=========================================================================== Clock Configuration
// <e0.8> IRQ 8-15 Channels: Non-system-handler (external/maskable) Interrupts
// <e0.0> 8: EXTI2 (External Interrupt Line 2) Interrupt Priority 
//      <o1.0..3> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.0..3> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.0> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 8
//-------------------------------------------------------------
// <e0.1> 9: EXTI3 (External Interrupt Line 3) Interrupt Priority 
//      <o1.4..7> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.4..7> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.1> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 9
//-------------------------------------------------------------
// <e0.2> 10: EXTI4 (External Interrupt Line 4) Interrupt Priority
//      <o1.8..11> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.8..11> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.2> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 10
//-------------------------------------------------------------
// <e0.3> 11: DMA1_Channel1 (DMA1 Channel 1 global) Interrupt Priority
//      <o1.12..15> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.12..15> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.3> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 11
//-------------------------------------------------------------
// <e0.4> 12: DMA1_Channel2 (DMA1 Channel 2 global) Interrupt Priority 
//      <o1.16..19> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.16..19> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.4> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 12
//-------------------------------------------------------------
// <e0.5> 13: DMA1_Channel3 (DMA1 Channel 3 global) Interrupt Priority
//      <o1.20..23> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.20..23> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.5> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 13
//-------------------------------------------------------------
// <e0.6> 14: DMA1_Channel4 (DMA1 Channel 4 global) Interrupt Priority
//      <o1.24..27> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o1.24..27> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.6> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 14
//-------------------------------------------------------------
// <e0.7> 15: DMA1_Channel5 (DMA1 Channel 5 global) Interrupt Priority
//      <o1.28..31> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.28..31> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.7> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 15
//-------------------------------------------------------------
// </e>
//
#define __SETUP_CHL8_15	0x0000
#define __PREP_CHL8_15	0x00000000
#define __SUBP_CHL8_15	0x00000000
#define __ENABLE_CHL8_15	0x00

//=========================================================================== Clock Configuration
// <e0.8> IRQ 16-23 Channels: Non-system-handler (external/maskable) Interrupts
// <e0.0> 16: DMA1_Channel6 (DMA1 Channel 6 global) Interrupt Priority 
//      <o1.0..3> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.0..3> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.0> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 16
//-------------------------------------------------------------
// <e0.1> 17: DMA1_Channel7 (DMA1 Channel 7 global) Interrupt Priority 
//      <o1.4..7> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.4..7> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.1> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 17
//-------------------------------------------------------------
// <e0.2> 18: ADC1_2 (ADC1 and ADC2 global) Interrupt Priority
//      <o1.8..11> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.8..11> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.2> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 18
//-------------------------------------------------------------
// <e0.3> 19: USB_HP_CAN_TX (USB High Priority/ CAN TX) Interrupt Priority
//      <o1.12..15> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.12..15> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.3> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 19
//-------------------------------------------------------------
// <e0.4> 20: USB_LP_CAN_RX0 (USB Low Priority/ CAN RX0) Interrupt Priority 
//      <o1.16..19> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.16..19> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.4> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 20
//-------------------------------------------------------------
// <e0.5> 21: CAN_RX1 (CAN RX1) Interrupt Priority
//      <o1.20..23> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.20..23> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.5> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 21
//-------------------------------------------------------------
// <e0.6> 22: CAN_SCE (CAN SCE) Interrupt Priority
//      <o1.24..27> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o1.24..27> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.6> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 22
//-------------------------------------------------------------
// <e0.7> 23: EXTI9_5 (External Interrupt Lines 5 to 9) Interrupt Priority
//      <o1.28..31> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.28..31> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.7> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 23
//-------------------------------------------------------------
// </e>
//
#define __SETUP_CHL16_23	0x0104
#define __PREP_CHL16_23	0x00000100
#define __SUBP_CHL16_23	0x00000000
#define __ENABLE_CHL16_23	0x04

//=========================================================================== Clock Configuration
// <e0.8> IRQ 24-31 Channels: Non-system-handler (external/maskable) Interrupts
// <e0.0> 24: TIM1_BRK (TIM1 break) Interrupt Priority 
//      <o1.0..3> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.0..3> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.0> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 24
//-------------------------------------------------------------
// <e0.1> 25: TIM1_UP (TIM1 update) Interrupt Priority 
//      <o1.4..7> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.4..7> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.1> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 25
//-------------------------------------------------------------
// <e0.2> 26: TIM1_TRG_COM (TIM1 Trigger & Commutation) Interrupt Priority
//      <o1.8..11> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.8..11> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.2> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 26
//-------------------------------------------------------------
// <e0.3> 27: TIM1_CC (TIM1 capture compare) Interrupt Priority
//      <o1.12..15> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.12..15> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.3> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 27
//-------------------------------------------------------------
// <e0.4> 28: TIM2 (TIM2 global) Interrupt Priority 
//      <o1.16..19> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.16..19> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.4> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 28
//-------------------------------------------------------------
// <e0.5> 29: TIM3 (TIM3 global) Interrupt Priority
//      <o1.20..23> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.20..23> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.5> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 29
//-------------------------------------------------------------
// <e0.6> 30: TIM4 (TIM4 global) Interrupt Priority
//      <o1.24..27> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o1.24..27> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.6> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 30
//-------------------------------------------------------------
// <e0.7> 31: I2C1_EV (I2C1 event) Interrupt Priority
//      <o1.28..31> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.28..31> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.7> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 31
//-------------------------------------------------------------
// </e>
//
#define __SETUP_CHL24_31	0x0000
#define __PREP_CHL24_31	0x00020000
#define __SUBP_CHL24_31	0x00000100
#define __ENABLE_CHL24_31	0x14

//=========================================================================== Clock Configuration
// <e0.8> IRQ 32-39 Channels: Non-system-handler (external/maskable) Interrupts
// <e0.0> 32: I2C1_ER (I2C1 error) Interrupt Priority 
//      <o1.0..3> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.0..3> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.0> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 32
//-------------------------------------------------------------
// <e0.1> 33: I2C2_EV (I2C2 event) Interrupt Priority 
//      <o1.4..7> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.4..7> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.1> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 33
//-------------------------------------------------------------
// <e0.2> 34: I2C1_ER (I2C1 error) Interrupt Priority
//      <o1.8..11> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.8..11> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.2> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 34
//-------------------------------------------------------------
// <e0.3> 35: SPI1 (SPI1 global) Interrupt Priority
//      <o1.12..15> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.12..15> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.3> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 35
//-------------------------------------------------------------
// <e0.4> 36: SPI2 (SPI2 global) Interrupt Priority 
//      <o1.16..19> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.16..19> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.4> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 36
//-------------------------------------------------------------
// <e0.5> 37: USART1 (USART1 global) Interrupt Priority
//      <o1.20..23> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.20..23> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.5> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 37
//-------------------------------------------------------------
// <e0.6> 38: USART2 (USART2 global) Interrupt Priority
//      <o1.24..27> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o1.24..27> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.6> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 38
//-------------------------------------------------------------
// <e0.7> 39: USART3 (USART3 global) Interrupt Priority
//      <o1.28..31> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.28..31> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.7> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 39
//-------------------------------------------------------------
// </e>
//
#define __SETUP_CHL32_39	0x0000
#define __PREP_CHL32_39	0x00000000
#define __SUBP_CHL32_39	0x00000000
#define __ENABLE_CHL32_39	0x00

//=========================================================================== Clock Configuration
//=========================================================================== Clock Configuration
// <e0.8> IRQ 40-42 Channels: Non-system-handler (external/maskable) Interrupts
// <e0.0> 40: EXTI15_10 (External Interrupt Lines 15 to 10) Interrupt Priority 
//      <o1.0..3> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.0..3> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.0> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 40
//-------------------------------------------------------------
// <e0.1> 41: RTCAlarm (RTC alarm through EXTI line 17) Interrupt Priority 
//      <o1.4..7> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.4..7> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.1> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 41
//-------------------------------------------------------------
// <e0.2> 42: USBWakeup (USB wakeup from suspend through EXTI line 18) Interrupt Priority
//      <o1.8..11> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.8..11> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.2> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 42
//-------------------------------------------------------------
// </e>
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//=========================================================================== Clock Configuration
// <e0.12> For High-Density Device (256 to 512 KB embedded ROM)
//=========================================================================== Clock Configuration
// <e0.8> IRQ 43-47 Channels: Non-system-handler (external/maskable) Interrupts
//-------------------------------------------------------------
// <e0.3> 43: TIM8_BRK (TIM8 break) Interrupt Priority
//      <o1.12..15> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.12..15> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.3> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 43
//-------------------------------------------------------------
// <e0.4> 44: TIM8_UP (TIM8 update) Interrupt Priority 
//      <o1.16..19> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.16..19> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.4> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 44
//-------------------------------------------------------------
// <e0.5> 45: TIM8_TRG_COM (TIM8 trigger & commutation) Interrupt Priority
//      <o1.20..23> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.20..23> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.5> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 45
//-------------------------------------------------------------
// <e0.6> 46: TIM8_CC (TIM8 capture compare) Interrupt Priority
//      <o1.24..27> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o1.24..27> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.6> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 46
//-------------------------------------------------------------
// <e0.7> 47: ADC3 (ADC3 global) Interrupt Priority
//      <o1.28..31> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.28..31> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.7> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 47
//-------------------------------------------------------------
// </e>
//
#define __SETUP_CHL40_47	0x0901
#define __PREP_CHL40_47	0x00000002
#define __SUBP_CHL40_47	0x00000001
#define __ENABLE_CHL40_47	0x02

//=========================================================================== Clock Configuration
// <e0.8> IRQ 48-55 Channels: Non-system-handler (external/maskable) Interrupts
// <e0.0> 48: FSMC (FSMC global) Interrupt Priority 
//      <o1.0..3> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.0..3> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.0> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 48
//-------------------------------------------------------------
// <e0.1> 49: SDIO (SDIO global) Interrupt Priority 
//      <o1.4..7> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.4..7> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o1.1> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 49
//-------------------------------------------------------------
// <e0.2> 50: TIM5 (TIM5 global)) Interrupt Priority
//      <o1.8..11> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.8..11> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.2> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 50
//-------------------------------------------------------------
// <e0.3> 51: SPI3 (SPI3 global) Interrupt Priority
//      <o1.12..15> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.12..15> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.3> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 51
//-------------------------------------------------------------
// <e0.4> 52: USART4 (USART4 global) Interrupt Priority 
//      <o1.16..19> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.16..19> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.4> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 52
//-------------------------------------------------------------
// <e0.5> 53: USART5 (USART5 global) Interrupt Priority
//      <o1.20..23> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.20..23> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.5> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 53
//-------------------------------------------------------------
// <e0.6> 54: TIM6 (TIM6 global) Interrupt Priority
//      <o1.24..27> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o1.24..27> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.6> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 54
//-------------------------------------------------------------
// <e0.7> 55: TIM7 (TIM7 global) Interrupt Priority
//      <o1.28..31> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.28..31> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.7> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 55
//-------------------------------------------------------------
// </e>
//
#define __SETUP_CHL48_55	0x0000
#define __PREP_CHL48_55	0x00000000
#define __SUBP_CHL48_55	0x00000000
#define __ENABLE_CHL48_55	0x00

//=========================================================================== Clock Configuration
// <e0.8> IRQ 56-59 Channels: Non-system-handler (external/maskable) Interrupts
// <e0.0> 56: DMA2_Channel1 (DMA2 Channel 1 global) Interrupt Priority 
//      <o1.0..3> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.0..3> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.0> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 56
//-------------------------------------------------------------
// <e0.1> 57: DMA2_Channel2 (DMA2 Channel 2 global) Interrupt Priority  
//      <o1.4..7> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.4..7> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
///      <o3.1> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 57
//-------------------------------------------------------------
// <e0.2> 58: DMA2_Channel3 (DMA2 Channel 3 global) Interrupt Priority
//      <o1.8..11> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.8..11> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.2> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 58
//-------------------------------------------------------------
// <e0.3> 59: DMA2_Channel4_5 (DMA2 Channels 4 & 5 global) Interrupt Priority
//      <o1.12..15> Preemption Priority (0=higest) 
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o2.12..15> SubPriority (0=higest) 
//           <i> NOT over (4-Preemption Priority Bits) bits
//                       <0=> 0 (highest)
//                       <1=> 1 (1 bit)
//                       <2=> 2 (2 bits)
//                       <3=> 3 (2 bits)
//                       <4=> 4 (3 bits)
//                       <5=> 5 (3 bits)
//                       <6=> 6 (3 bits)
//                       <7=> 7 (3 bits)
//                       <8=> 8 (4 bits)
//                       <9=> 9 (4 bits)
//                       <10=> 10 (4 bits)
//                       <11=> 11 (4 bits)
//                       <12=> 12 (4 bits)
//                       <13=> 13 (4 bits)
//                       <14=> 14 (4 bits)
//                       <15=> 15 (4 bits)
//      <o3.3> NVIC Enable Bit
//                       <0=> NOT change
//                       <1=> set enable
// </e> End of 59
//-------------------------------------------------------------
// </e>
//
#define __SETUP_CHL56_59	0x0000
#define __PREP_CHL56_59	0x00000000
#define __SUBP_CHL56_59	0x00000000
#define __ENABLE_CHL56_59	0x00


// </e> END of For High-Density Device (256 to 512 KB embedded ROM)
//-------- <<< end of configuration section >>> ---------------

/*----------------------------------------------------------------------------
 Check and correct the premption and subpriority values
 INPUTS: pre = preemption priority value
         sub = subpriority
 OUTPUT: (preN << (4-__PRE_BITS) | subN
     preN = pre; or the max. value can be represented by number of preemption bits if pre over that max. value
     subN = sub; or the max. value can be represented by number of subpriority bits if sub over that max. value
 *----------------------------------------------------------------------------*/
#define BITS_of_TOTAL_PRIPORITY	4		// 4-bit priority 
__inline static u32 correct_pri(u32 pre, u32 sub)
{
 if (pre < ( 1<<__PRE_BITS ) )
	pre = pre <<  ( (4-__PRE_BITS)+(8-BITS_of_TOTAL_PRIPORITY) );
 else
	pre = ( ((u8)1<<__PRE_BITS)-1 ) << ( (4-__PRE_BITS)+(8-BITS_of_TOTAL_PRIPORITY) );


 if ( sub < ( 1<<(4 - __PRE_BITS)) )
	pre |= sub << (8-BITS_of_TOTAL_PRIPORITY);
 else
	pre |= ( ((u8)1<<__PRE_BITS)-1 ) << (8-BITS_of_TOTAL_PRIPORITY);

	return pre;
}

/*----------------------------------------------------------------------------
 set interrupt priority for 8 non-system channels per one time
 INPUTS: setup_index = (0<<8)| (__SETUP_CHL0_7 & 0xFF);
	     preX:  __PREP_CHLx_y, 
		 subX:  __SUBP_CHLx_y, 
		 enableX: __ENABLE_CHLx_y
 OUTPUT: none
 *----------------------------------------------------------------------------*/
void Set_8chls_Priority(u32 setup_index, u32 preX, u32 subX, u32 enableX){
	u32 i, j, pri;
	u8* pD;

	pD = (u8 *) &NVIC->IP[0];
           /* One IPR contains 4 bIP's (8 bits) for 4 IRQ¡¦s.  */
           /*  STM32 has only 4-bit priority, 4 LSB should be 0 */
	for (i=0; i<8; i++){
	  if (setup_index &  Bit(i) ){
	    pri = correct_pri( (preX>>(4*i))& 0x0F, (subX>>(4*i)) & 0x0F);
 
  //== Set Priority for IRQ 0 Interrupts ==
	    j = i+ (setup_index>>8);
	    pD[j] = (u8) pri;                  // set priority (4-bit level) 

 	    if (enableX & Bit(i) ) {
       //== Each ICER/ISER has 32 bits corresponding to 32 IRQ 
	      NVIC->ICPR[j>>5] = Bit( (j&0x1F) );	 // clear pending interrupt IRQ[j]
	      NVIC->ISER[j>>5] = Bit( (j&0x1F) );	 // enable interrupt IRQ[j]
	    }
	  }	 // END of if (setup_index &  Bit(i) )
	}  // END of   for (i=0; i<8; i++)
}	    


/*###########################################################################
 STM32 initialization
 Call this function as first in main ()
 *###########################################################################*/
void stm32_nvic () {
	u32 pri, setup_index;

/*----------------------------------------------------------------------------
 setting number of bits for
 STM32 Interrupt Preemption Priority.
 *----------------------------------------------------------------------------*/
	SCB->AIRCR = 0x05FA0000 | ((7-__PRE_BITS)<<8);  /* 0x05FA0000: writing this register requires 0x05FA in Field "VECTKEY¡§ */

/*----------------------------------------------------------------------------
 STM32 System Timer Interrupt Priority.
 *----------------------------------------------------------------------------*/
#if __SysTick_SETUP
	pri = correct_pri(__SysTick_PREP, __SysTick_SUBP);
 
	SCB->SHP[2] &= ~((u32) 0xFF<<24);   // PRI_15: SysTick interrupt
	SCB->SHP[2] |=  pri<<(24);   

  #if __SysTick_ENABLE
	SysTick->CTRL |= SYSTICK_CSR_ENABLE;                          // enable the counter
  #endif
#endif

//************** IRQ 0-7 ***********************************
#if (__SETUP_CHL0_7 & 0x0100 )
	setup_index = (0<<8)| (__SETUP_CHL0_7 & 0xFF);
	Set_8chls_Priority(setup_index, __PREP_CHL0_7, __SUBP_CHL0_7, __ENABLE_CHL0_7);
#endif

//************** IRQ 8-15 ***********************************
#if (__SETUP_CHL8_15 & 0x0100 )
	setup_index = (8<<8)| (__SETUP_CHL8_15 & 0xFF);
	Set_8chls_Priority(setup_index, __PREP_CHL8_15, __SUBP_CHL8_15, __ENABLE_CHL8_15);
#endif

//************** IRQ 16-23 ***********************************
#if (__SETUP_CHL16_23 & 0x0100 )
	setup_index = (16<<8)| (__SETUP_CHL16_23 & 0xFF);
	Set_8chls_Priority(setup_index, __PREP_CHL16_23, __SUBP_CHL16_23, __ENABLE_CHL16_23);
#endif

//************** IRQ 24-31 ***********************************
#if (__SETUP_CHL24_31 & 0x0100 )
	setup_index = (24<<8)| (__SETUP_CHL24_31 & 0xFF);
	Set_8chls_Priority(setup_index, __PREP_CHL24_31, __SUBP_CHL24_31, __ENABLE_CHL24_31);
#endif

//************** IRQ 32-39 ***********************************
#if (__SETUP_CHL32_39 & 0x0100 )
	setup_index = (32<<8)| (__SETUP_CHL0_7 & 0xFF);
	Set_8chls_Priority(setup_index, __PREP_CHL32_39, __SUBP_CHL32_39, __ENABLE_CHL32_39);
#endif

//************** IRQ 40-47 ***********************************
#if (__SETUP_CHL40_47 & 0x0100 )
	setup_index = (40<<8)| (__SETUP_CHL40_47 & 0xFF);
	Set_8chls_Priority(setup_index, __PREP_CHL40_47, __SUBP_CHL40_47, __ENABLE_CHL40_47);
#endif

#if (__SETUP_CHL40_47 & 0x1000 )       // bit 12 = 1: high-density devices
//************** IRQ 48-55 ***********************************
#if (__SETUP_CHL48_55 & 0x0100 )
	setup_index = (48<<8)| (__SETUP_CHL48_55 & 0xFF);
	Set_8chls_Priority(setup_index, __PREP_CHL48_55, __SUBP_CHL48_55, __ENABLE_CHL48_55);
#endif

//************** IRQ 56-59 ***********************************
#if (__SETUP_CHL56_59 & 0x0100 )
	setup_index = (56<<8)| (__SETUP_CHL56_59 & 0xFF);
	Set_8chls_Priority(setup_index, __PREP_CHL56_59, __SUBP_CHL56_59, __ENABLE_CHL56_59);
#endif

#endif // END of #if (__SETUP_CHL40_47 & 0x1000 )
} // end of stm32_nvic

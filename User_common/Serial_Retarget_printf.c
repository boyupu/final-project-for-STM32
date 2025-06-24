// Modified by Shir-Kuan Lin
//             Department of Electrical Engineering
//             National Chiao Tung University
//             email: sklin@mail.nctu.edu.tw

/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low level serial routines for STM32
 * Version: V1.00
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * Copyright (c) 2005-2007 Keil Software. All rights reserved.
 *----------------------------------------------------------------------------*/

//################################################
// Wizard
// by Shir-Kuan Lin
//################################################
//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//

//=========================================================================== USART Configuration
// <o0> "printf" output Configuration
//                     <1=> __DBG_ITM
//                     <2=> USART1
//                     <4=> USART2
//                     <8=> USART3
//--------------------------------------------------------------------------- USART1
#define __printf_SETUP           8                       //  0

//--------<<< end of configuration section >>>

#include "stm32f4xx.h"

#if (__printf_SETUP &0x01)
#define __DBG_ITM
#else
  #if (__printf_SETUP &0x02)
  #define USARTx   USART1                           // USART1 is used
  #endif

  #if (__printf_SETUP &0x04)
  #define USARTx   USART2                           // USART2 is used
  #endif

  #if (__printf_SETUP &0x08)
  #define USARTx   USART3                           // USART3 is used
  #endif
#endif

/* NOTE: Remark!!!!!===================
#define USART_FLAG_TXE              ((uint16_t)1<<7)//; 0x0080
#define USART_FLAG_RXNE             ((uint16_t)1<<5)//; 0x0020
*/

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif

#define USART_FLAG_TXE              ((uint16_t)1<<7)//; 0x0080
#define USART_FLAG_RXNE             ((uint16_t)1<<5)//; 0x0020

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int sendchar (int c) {

#ifdef __DBG_ITM
  ITM_SendChar(c);
#else
  while (!(USARTx->SR & USART_FLAG_TXE)){}
  if (c == '\n')  
	{
    USARTx->DR = 0x0D;
  }else
	{
		USARTx->DR = (c & 0x1FF);    // just in case of the word length being 9 bits
	}
#endif

  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int getkey (void) {

#ifdef __DBG_ITM
  while (ITM_CheckChar() != 1) __NOP();
  return (ITM_ReceiveChar());
#else
  while (!(USARTx->SR & USART_FLAG_RXNE));

  return ((int)(USARTx->DR & 0x1FF));  // just in case of the word length being 9 bits
#endif
}


//=========== RETARGET ======================

#include <stdio.h>
//struct __FILE { int handle; }; // Add whatever you need here 
FILE __stdout;
FILE __stdin;


int fputc(int ch, FILE *f) {
  return (sendchar(ch));
}


int fgetc(FILE *f) {
  return (getkey());
}


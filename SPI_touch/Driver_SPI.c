#include "stm32f4xx.h"
#ifndef Bit
#define Bit(x) 	(0x01ul<<x)
#endif
//==== for Board B ================
#define CS_pin 	Bit(13)	// PC13
#define SCK_pin 	Bit(0)	// PB0
#define MISO_pin 	Bit(2)	// PB2
#define MOSI_pin 	Bit(11)	// PF11
#define GPIO_MI 	GPIOB	// PB2
#define GPIO_MO 	GPIOF	// PF11
#define GPIO_SCK 	GPIOB	// PB0
#define GPIO_CS 	GPIOC	// PC13

#define pin_0 0
#define pin_1 1
#define pin_2 2
#define pin_3 3
#define pin_11 11
#define pin_13 13
#define bit_GPIOA 0
#define bit_GPIOB 1
#define bit_GPIOF	5 // pin 1, pin 2, GPIOB, GPIOF
#define bit_GPIOC 2
#define bit_GPIOD 3
void Driver_GPIO(void)
{
/*  --------------------------------------------------------------
    GPIOA: INPUT, pull-down
    --------------------------------------------------------------
    | Pin : PA |    0
    | Key 1    |   KEY1
   --------------------------------------------------------------
*/
   RCC->AHB1ENR |=  (1UL << bit_GPIOA);     // enable clock for GPIOA
 // 0:PORTA, 1: PORTB, ?, 10: PORTK
   GPIOA->MODER   = (GPIOA->MODER & ~(0x03ul<<(pin_0 *2)) );                  //Input mode (00b)
    GPIOA->PUPDR   = (GPIOA->PUPDR & ~(0x03ul<<(pin_0 *2)) ) | (0x02ul<<(pin_0 *2)); //pull-down (10b)
    GPIOA->AFR[0]  &= ~(0x0Ful<<pin_0 *4);           // AF0 (0000b)
/*  --------------------------------------------------------------
    GPIOF: INPUT, floating (no Pull-Up or Pull-Down) 
   --------------------------------------------------------------
   | Pin : PB           |    1     
   | touch Module  |   TP_INT  
   --------------------------------------------------------------
*/
RCC->AHB1ENR |=  (1UL << bit_GPIOB);     // enable clock 
 // 0:PORTA, 1: PORTB, ?, 10: PORTK
    GPIOB->MODER   = (GPIOB->MODER & ~(0x03ul<<(pin_1 *2)) );                  //Input mode (00b)
    GPIOB->PUPDR   = (GPIOB->PUPDR & ~(0x03ul<<(pin_1 *2)) ); //floating (00b)
    GPIOB->AFR[0]  &= ~(0x0Ful<<pin_1 *4);           // AF0 (0000b)
/*  --------------------------------------------------------------
    GPIOC: OUTPUT, Speed_25MHz medium, push pull, no pull 
   --------------------------------------------------------------
    | Pin: PC |    13       
    | SPI       |    CS   
    | initial   |   1(high)
    --------------------------------------------------------------
*/
    RCC->AHB1ENR |=  (1UL << bit_GPIOC);     // enable clock for GPIOC
    // 0:PORTA, 1: PORTB, ..., 10: PORTK
    GPIOC->BSRR = Bit(pin_13);			// Initial output value for CS = 1
    GPIOC->MODER   = (GPIOC->MODER & ~(0x03ul<<(pin_13 *2)) )  | (0x01ul<<(pin_13 *2)); //Output mode (01b)
    GPIOC->OSPEEDR = (GPIOC->OSPEEDR & ~(0x03ul<<(pin_13 *2)) ) |  (0x01ul<<(pin_13 *2)); //medium speed (01b)
    GPIOC->OTYPER  &= ~(0x01ul<<pin_13);         		// push-pull (0b)
    GPIOC->PUPDR   = (GPIOC->PUPDR & ~(0x03ul<<(pin_13 *2)) ); // no PU, no PD (00b)
    GPIOC->AFR[1]  &= ~(0x0Ful<<((pin_13 -8)*4));      // AF0 (0000b)
/*  --------------------------------------------------------------
    GPIOB: OUTPUT, Speed_25MHz medium, push pull, no pull 
    --------------------------------------------------------------
   | Pin: PB |     0      
    | SPI       |    SCK   
    | initial   |   0 (low) 
    --------------------------------------------------------------
*/
   RCC->AHB1ENR |=  (1UL << bit_GPIOB);     // enable clock for GPIOB
    // 0:PORTA, 1: PORTB, ..., 10: PORTK
  GPIOB->BSRR = (1UL << (pin_0 + 16));	// Initial output value for SCK = 0
	GPIOB->MODER   = (GPIOB->MODER & ~(0x03ul<<(pin_0 *2)) )  | (0x01ul<<(pin_0 *2)); //Output mode (01b)
    GPIOB->OSPEEDR = (GPIOB->OSPEEDR & ~(0x03ul<<(pin_0 *2)) ) |  (0x01ul<<(pin_0 *2)); //medium speed (01b)
    GPIOB->OTYPER  &= ~(0x01ul<<pin_0);         		// push-pull (0b)
    GPIOB->PUPDR   = (GPIOB->PUPDR & ~(0x03ul<<(pin_0 *2)) ); // no PU, no PD (00b)
    GPIOB->AFR[0]  &= ~(0x0Ful<<((pin_0 )*4));      // AF0 (0000b)
    

/*  --------------------------------------------------------------
    GPIOF: OUTPUT, Speed_25MHz medium, push pull, no pull 
    --------------------------------------------------------------
    | Pin: PF |    11   
    | SPI       |  MOSI
    | initial   |   x (not care)
     --------------------------------------------------------------
*/
RCC->AHB1ENR |=  (1UL << bit_GPIOF);     // bit_GPIOF: enable clock for GPIOF
   // 0:PORTA, 1: PORTB, ?, 10: PORTK
    GPIOF->MODER   = (GPIOF->MODER & ~(0x03ul<<(pin_11 *2)) )  | (0x01ul<<(pin_11 *2)); //Output mode (01b)
    GPIOF->OSPEEDR = (GPIOF->OSPEEDR & ~(0x03ul<<(pin_11 *2)) ) |  (0x01ul<<(pin_11 *2)); //medium speed (01b)
    GPIOF->OTYPER  &= ~(0x01ul<<pin_11);         		// push-pull (0b)
    GPIOF->PUPDR   = (GPIOF->PUPDR & ~(0x03ul<<(pin_11 *2)) ); // no PU, no PD (00b)
    GPIOF->AFR[1]  &= ~(0x0Ful<<((pin_11 -8)*4));      // AF0 (0000b)

/*  --------------------------------------------------------------
    GPIOB: INPUT, float 
    --------------------------------------------------------------
    | Pin: PB |      2   
    | SPI        |    MISO  
    --------------------------------------------------------------
*/
RCC->AHB1ENR |=  (1UL << bit_GPIOB);     // enable clock for GPIOA
 // 0:PORTA, 1: PORTB, ?, 10: PORTK
   GPIOB->MODER   = (GPIOB->MODER & ~(0x03ul<<(pin_2 *2)) );                  //Input mode (00b)
    GPIOB->PUPDR   = (GPIOB->PUPDR & ~(0x03ul<<(pin_2 *2)) ) | (0x02ul<<(pin_2 *2)); //pull-down (10b)
    GPIOB->AFR[0]  &= ~(0x0Ful<<pin_2 *4);           // AF0 (0000b)

}			// end of Driver_GPIO

typedef unsigned char       bool;
void Driver_SPICS(bool sta);
void Touch_SPIWrite(uint8_t Val);
uint16_t Touch_SPIRead(void);

/***********************************************
*  Output Function: Driver_SPICS	(CS pin)
*  Object: touch ic spi enable/unable
*  brief:	set CS = 0 if t_f=0; CS = 1 if t_f = 1.
***********************************************/
void Driver_SPICS(bool t_f)
{
if (!t_f) 	// t_f = 0
  GPIO_CS->BSRR = CS_pin<<16;	// reset ==> 0
else	// t_f = 1
  GPIO_CS->BSRR = CS_pin;		// set ==> 1
}

/***********************************************
*  Output Function: Driver_SPISCK	(SCK pin)
*  Object: touch spi clock output
*  brief:	set SCK = 0 if t_f=0; SCK = 1 if t_f = 1.
***********************************************/
void Driver_SPISCK(bool t_f)
{
if (!t_f)
        GPIO_SCK->BSRR = SCK_pin << 16;	// Reset pin
    else
        GPIO_SCK->BSRR = SCK_pin;	// Set pin
}

/***********************************************
*  Output Function: Driver_SPIMOSI	(MOSI pin)
*  Object: master out
*  brief:	set MOSI = 0 if t_f=0; MOSI = 1 if t_f = 1.
***********************************************/
void Driver_SPIMOSI(bool t_f)
{
if (!t_f)
        GPIO_MO->BSRR = MOSI_pin << 16;	// Reset pin
    else
        GPIO_MO->BSRR = MOSI_pin;	// Set pin
}

/***********************************************
*  Input Function: Driver_SPIMISO	(MISO pin)
*  Object: master in
*  Return: 1 if MISO=1; 0 if MISO = 0 
***********************************************/
bool Driver_SPIMISO(void)
{
 if (GPIO_MI->IDR & MISO_pin)	// if MISO pin = 1
	return 1;
else
	return 0;
}
void Delay_SPI(uint16_t Num)
{
	volatile uint16_t Timer;
	while(Num--)
	{
	 	Timer = 20;
		while(Timer--);
	}
}

//--------- Receive Data from SPI
//#define Test_Normalcase
uint16_t Touch_SPIRead(void)
{
    uint8_t i;
    uint16_t Val = 0;

//	for(i=0; i<12; i++)			// 12-bit data: max value = 4095
	  // the least 4 significant bits are ignorable
	for(i = 0; i < 16; i++)	// Read 16 bits
    {
        Val <<= 1;
#ifndef Test_Normalcase
			Driver_SPISCK(1);
			Delay_SPI(1);
#endif
        Driver_SPISCK(0);      // Clock low
        Delay_SPI(1);
        Driver_SPISCK(1);      // Clock high: slave outputs data now
        if (Driver_SPIMISO())  // Read MISO
            Val++;
        Delay_SPI(1);
#ifdef Test_Normalcase
			Driver_SPISCK(1);
			Delay_SPI(1);
#endif
    }
#ifdef Test_Normalcase
			Driver_SPISCK(0);
			Delay_SPI(1);
#endif		
    
    return Val;
}
//--------- Send data to SPI
void Touch_SPIWrite(uint8_t Val)
{
    uint8_t i;
    Driver_SPISCK(0);  // make sure that the idle clock is low
    for(i=0; i<8; i++)		// 8 bit mode
	{
	if(Val & Bit(7)) // most significant bit first
	 Driver_SPIMOSI(1); // if Bit value = 1
	else
	 Driver_SPIMOSI(0); // if Bit value = 0
	Val <<= 1;
 Driver_SPISCK(0); // load at the FALLing edge of SCK clock
	Delay_SPI(1);	 // hold half period
 Driver_SPISCK(1); // capture at the RISing edge of SCK Clock
	Delay_SPI(1);	 // hold half period
    }
Driver_SPISCK(0); 		// idle mode
Delay_SPI(1);
}
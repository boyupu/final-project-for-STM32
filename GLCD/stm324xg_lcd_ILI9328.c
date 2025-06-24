
//-------- <<<  Menu >>> -----------------
//
//=================================================
#define  RS_bit							 6
#define	 LCD_BASE_sel        0x6C000000
#define	 LCD_BASE        (LCD_BASE_sel | (0x01UL<<(RS_bit+1))-2)
//=============================================

//==============================================================
#define __Fonts_select         0x0000001E			//0
#define __default_Font         3			//1
#define __LCD_DIRECT	         1      //2 ==> 1: LANDSCAPEそ  2: PORTRAIT
//=============================================

//=================================================
#define USE_resetPIN 0		// 0
#define PNr_reset 2		// 1
#define PIN_rs	5     // 2
#define PNr_backlight 1		// 3
#define PIN_bl	15         // 4
//=============================================

//=================================================
#define PNr_FSMC_set1 3									// 0
#define PinBit_FSMC_set1 0xC733     		// 1
#define PNr_FSMC_set2 4									// 2
#define PinBit_FSMC_set2	0xFF80        // 3
#define PNr_FSMC_set3 6									// 4
#define PinBit_FSMC_set3	0x1000        // 5
#define PNr_FSMC_set4 5									// 6
#define PinBit_FSMC_set4	0x1000        // 7
//=============================================

//=================================================
#define Default_TextColor		0xFFFF
#define Default_BackColor		0x0000
//=============================================

//=================================================
#define Show_DeviceID		1
//=============================================

//-------- <<< end  >>> ---------------

/* Includes ------------------------------------------------------------------*/
#include "stm324xg_lcd_sklin.h"
//#define USE_HAL_LL
#ifndef USE_HAL_LL
#include "stm32f4xx_fsmc.h"
#else
#include "stm32f4xx_ll_fsmc.h"
#endif


#ifndef Bit
#define Bit(x) 	(0x01ul<<x)
#endif

/** 
  * @brief LCD default font 
  */ 
#if (__default_Font == 0)
	#define LCD_DEFAULT_FONT         Font8
#endif
#if (__default_Font == 1)
	#define LCD_DEFAULT_FONT         Font12
#endif
#if (__default_Font == 2)
	#define LCD_DEFAULT_FONT         Font16
#endif
#if (__default_Font == 3)
	#define LCD_DEFAULT_FONT         Font20
#endif
#if (__default_Font == 4)
	#define LCD_DEFAULT_FONT         Font24
#endif
	
#define _fontSel   (0x01<<(__default_Font) | __Fonts_select) 
#if (_fontSel & Bit(0))
	#include ".\Fonts\font8.c"
#endif
#if (_fontSel & Bit(1))
	#include ".\Fonts\font12.c"
#endif
#if (_fontSel & Bit(2))
	#include ".\Fonts\font16.c"
#endif
#if (_fontSel & Bit(3))
	#include ".\Fonts\font20.c"
#endif
#if (_fontSel & Bit(4))
	#include ".\Fonts\font24.c"
#endif


#define ABS(X)  ((X) > 0 ? (X) : -(X))
#define MAX_POLY_CORNERS   200
#define POLY_Y(Z)          ((int32_t)((Points + Z)->X))
#define POLY_X(Z)          ((int32_t)((Points + Z)->Y))

/* Global variables to set the written text color */
typedef struct 
{ 
  uint16_t TextColor;
  uint16_t BackColor;
  sFONT    *pFont; 
}LCD_DrawPropTypeDef;

static LCD_DrawPropTypeDef DrawProp={ \
			Default_TextColor, Default_BackColor, &LCD_DEFAULT_FONT};

//*** Variables concerning FONT ************
#define	pLCD_Currentfonts DrawProp.pFont
static uint16_t wf, hf;	// font_width, font_height
static uint16_t bytes_f, bytes_wf;	// number of bytes per font, that per width line in a font
volatile uint16_t lcd_id = 0;
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed);

//########################################################
//########################################################
//  DRIVER 													 
//########################################################
//########################################################
#if __LCD_DIRECT==1
  #define  LANDSCAPE			1
	#define  LCD_ROW_NUM    240                //pages in ILI9341 or ILI9328
	#define  LCD_COL_NUM    320                //columns in ILI9341 or ILI9328
#else
  #define  LANDSCAPE			0
	#define  LCD_ROW_NUM    320                //pages
	#define  LCD_COL_NUM    240                //columns
#endif
	#define  LCD_PIXEL_WIDTH   (LCD_COL_NUM -1)  
	#define  LCD_PIXEL_HEIGHT  (LCD_ROW_NUM -1)  

uint16_t LCD_Pixel_Width(void){ return LCD_PIXEL_WIDTH;}
uint16_t LCD_Pixel_Height(void){ return LCD_PIXEL_HEIGHT;}

typedef struct
{
  volatile uint16_t LCD_REG;			// RS = 0
  volatile uint16_t LCD_RAM;      // RS = 1
} LCD_TypeDef;		


	GPIO_TypeDef *GPIO_backlight, *GPIO_reset;
//#define	 LCD_BASE        ((uint32_t)(0x6C000000 | 0x000007FE))	// A10 as RS
#define  LCD             ((LCD_TypeDef *) LCD_BASE)
#define  LCD_turnON_backlight	 (GPIO_backlight->BSRR = Bit(PIN_bl))		// set: output 1
#define  LCD_turnOFF_backlight (GPIO_backlight->BSRR = Bit(PIN_bl)<<16)		// reset: output 0
#define	 LCD_RESET_H  		(GPIO_reset->BSRR = Bit(PIN_rs))
#define	 LCD_RESET_L  		(GPIO_reset->BSRR = Bit(PIN_rs)<<16)
//<<<<<<<<<<<

#define LCD_WriteReg(x)	(LCD->LCD_REG = (uint16_t) x)	//lcd write reg
#define LCD_WriteData(x)	(LCD->LCD_RAM = (uint16_t) x)	//lcd write data
#define LCD_ReadData	((uint16_t) LCD->LCD_RAM)	//lcd read data

/********************************************************************************************************
// Display Inversion On & Off
// ILI9328 Code
********************************************************************************************************/
#define ReverseLCD()		LCD_WriteRegX(0x61, 0x0000)	// Display Inversion ON (21h)
#define NormalLCD()			LCD_WriteRegX(0x61, 0x0001)	// Display Inversion ON (21h)

/***********************************************
  * @brief  Configures the Parallel interface (FSMC) for LCD(Parallel mode)
  * @param  None
  * @retval None
  */
void LCD_FSMCConfig(void)
{
	FSMC_NORSRAM_InitTypeDef  hsram4_Init;
	FSMC_NORSRAM_TimingTypeDef wr_Timing, w_Timing;
	uint32_t FSMC_Bank1_NEx;
//	#define FSMC_NORSRAM_DEVICE                   FSMC_Bank1
//  #define FSMC_NORSRAM_EXTENDED_DEVICE 					FSMC_Bank1E
		FSMC_Bank1_NEx = ((LCD_BASE_sel>>26) & 0x03 )*2;	// for field of BCTR[(NEx-1)*2]
	// 0= NE1; 2= NE2; 4= NE3; 6= NE4 (=FSMC_NORSRAM_BANK4)
	
  /* Enable FSMC clock */
//  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
		RCC->AHB3ENR |= Bit(0); //bit 0: RCC_AHB3Periph_FSMC


/*-- FSMC Configuration ------------------------------------------------------*/
/*----------------------- SRAM Bank 1 ----------------------------------------*/
  /* FSMC_Bank1 NEx (x=1,..,4) configuration */
	/* Color LCD configuration ------------------------------------
     LCD (FSMC) configured as follow:
        - Data/Address MUX = Disable
        - Memory Type = SRAM
        - Data Width = 16bit //<<<<<============
        - Write Operation = Enable
        - Extended Mode = Disable
        - Asynchronous Wait = Disable */

//====================================
	hsram4_Init.PageSize  = 0;   // extraneous and useless field
//====================================

	hsram4_Init.NSBank = FSMC_Bank1_NEx; // for field of BCTR[(NEx-1)*2]
  hsram4_Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram4_Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram4_Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram4_Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram4_Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram4_Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram4_Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram4_Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram4_Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram4_Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;		// 0x00000000U
	//disable: Device->BWTR[Bank] = 0x0FFFFFFFU;  //Device=FSMC_Bank1e,
  hsram4_Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram4_Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;

	wr_Timing.AddressSetupTime = 11;		// templet: 15
  wr_Timing.AddressHoldTime = 0;
  wr_Timing.DataSetupTime = 11;		// templet: 60
  wr_Timing.BusTurnAroundDuration = 0;
  wr_Timing.CLKDivision = 0;
  wr_Timing.DataLatency = 0;
  wr_Timing.AccessMode = FSMC_ACCESS_MODE_A;

  // Initialize SRAM control Interface BCTR[(NEx-1)*2]
  FSMC_NORSRAM_Init(FSMC_Bank1, &hsram4_Init);
	// FSMC_Bank1: FSMC Bank1 registers base address = 0xA0000000UL

  // Initialize SRAM timing Interface BCTR[(NEx-1)*2 + 1]
  FSMC_NORSRAM_Timing_Init(FSMC_Bank1, &wr_Timing, FSMC_Bank1_NEx); 

  if(hsram4_Init.ExtendedMode == FSMC_EXTENDED_MODE_ENABLE)
  {
		w_Timing.AddressSetupTime = 9;
		w_Timing.AddressHoldTime = 0;
		w_Timing.DataSetupTime = 8;
		w_Timing.BusTurnAroundDuration = 0;
		w_Timing.CLKDivision = 0;
		w_Timing.DataLatency = 0;
		w_Timing.AccessMode = FSMC_ACCESS_MODE_A;
  // Initialize SRAM extended mode timing Interface BWTR[(NEx-1)*2]
		FSMC_NORSRAM_Extended_Timing_Init(FSMC_Bank1E, &w_Timing, FSMC_Bank1_NEx,  FSMC_EXTENDED_MODE_ENABLE);  
  } else
	{
		    FSMC_Bank1E->BWTR[FSMC_Bank1_NEx] = 0x0FFFFFFFU;	// reset value
	}
  /* Enable FSMC NOR/SRAM Bank1 */
		__FSMC_NORSRAM_ENABLE(FSMC_Bank1, FSMC_Bank1_NEx); 
		//FSMC_Bank1->BTCR[FSMC_Bank1_NEx] |=  FSMC_BCR1_MBKEN;  // bit0: FSMC_BCR1_MBKEN;
}

/********************************************************************************************************
*  Function: GPIO_outPPhigh				                                                           
*  Object: set GPIO pin as output, push-pull, high speed (100 MHz)
*  Input: PortNum (0, ..., 6); PinNum (0, ..., 15)
*  Output: GPIO_x                                  
********************************************************************************************************/
static GPIO_TypeDef* GPIO_outPPhigh(uint32_t PortNum, uint32_t PinNum)
{
	GPIO_TypeDef *GPIO_x;
	uint32_t y=PinNum;

		RCC->AHB1ENR |=  (1UL << PortNum);     // enable clock for GPIOx
    // __SPI1_CS_PORT = 0: PORTA, 1: PORTB, ..., 10: PORTK
    GPIO_x  = (GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x0400 * PortNum);

    GPIO_x->MODER   = (GPIO_x->MODER & ~Bit((y*2+1)) )  | Bit(y*2); //Output mode (01b)
    GPIO_x->OSPEEDR = (GPIO_x->OSPEEDR |  (0x03ul<<(y*2)) ); //high speed (11b)
    GPIO_x->OTYPER  &= ~Bit(y);                      // push-pull (0b)
    GPIO_x->PUPDR   &= ~(0x03ul<<(y*2));       //NO pull (00b)
  if (y < 8)
    GPIO_x->AFR[0]  &= ~(0x0Ful<<(y*4));      // AF0
  else
    GPIO_x->AFR[1]  &= ~(0x0Ful<<((y-8)*4));  // AF0
	
		return GPIO_x;
}

/********************************************************************************************************
*  Function: GPIO_AF12PPhigh				                                                           
*  Object: set GPIO pin as output, push-pull, high speed (100 MHz)
*  Input: PortNum (0, ..., 6); PinNum (0, ..., 15)
*  Output: none                                  
********************************************************************************************************/
static void GPIO_AF12PPhigh(uint32_t PortNum, uint32_t PinBit)
{
	GPIO_TypeDef *GPIO_x;
	uint32_t temp, chg1= PinBit, chg2=0, chg4L=0, chg4H=0;
	uint32_t fill2=0, fill4L=0, fill4H=0;

		RCC->AHB1ENR |=  (1UL << PortNum);     // enable clock for GPIOx
    // __SPI1_CS_PORT = 0: PORTA, 1: PORTB, ..., 10: PORTK
    GPIO_x  = (GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x0400 * PortNum);

  
	for (temp=0; temp<16; temp++){
			if (PinBit & 0x01){
			   chg2 |= (0x03ul) << (temp*2);
			   fill2 |= (0x02ul) << (temp*2);
			  if (temp <8){
			   chg4L |= (0x0Ful) << (temp*4);
			   fill4L |= (0x0Cul) << (temp*4);
			  }else {
			   chg4H |= (0x0Ful) << ((temp-8)*4);
			   fill4H |= (0x0Cul) << ((temp-8)*4);
			  }				
		  }
			PinBit = PinBit >> 1;
	}

		GPIO_x->MODER   = (GPIO_x->MODER & ~chg2)  | fill2; //AF mode (10b)
    GPIO_x->OSPEEDR = (GPIO_x->OSPEEDR |  chg2 ); //high speed (11b)
    GPIO_x->OTYPER  &= ~chg1;                      // push-pull (0b)
    GPIO_x->PUPDR   &= ~chg2;       //NO pull (00b)
  if (chg4L)
	{
    temp = GPIO_x->AFR[0];
    temp &= ~chg4L;						// clear to 0000b
		GPIO_x->AFR[0] = (temp | fill4L);      // AF12  (1100b)
	}
	if (chg4H)
	{
    temp = GPIO_x->AFR[1];
    temp &= ~chg4H;						// clear to 0000b
		GPIO_x->AFR[1] = (temp | fill4H);      // AF12  (1100b)
	}
}

/********************************************************************************************************
*  Function: delay_ms				                                                           
*  Object: lcd init wait..
*  Input: Num
*  Output: none                                  
*  brief: time = Num * 1ms
********************************************************************************************************/
__WEAK void delay_ms(uint16_t Num)
{
		volatile uint16_t Timer;
		while(Num--)
		{
		 	Timer = 11500;
			while(Timer--); 
		}
}

/********************************************************************************************************
*  Function: LCD_Reset				                                                           
*  Object: lcd reset control
*  Input: none
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_Reset(void)
{
#if USE_resetPIN
		LCD_RESET_L;
		delay_ms(150);		// 150 ms
		LCD_RESET_H;
		delay_ms(50);	// 50 ms
#endif
}

void LCD_WriteRegX(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{	
		LCD_WriteReg(LCD_Reg);  // Register Code
		LCD_WriteData(LCD_RegValue); // Data Code
}	   

/********************************************************************************************************
*  Function: LCD_DisplayDevID				                                                           
*  Object: Display LCD device ID on the screen
*  Input: none
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_DisplayDevID()
{	
#include <stdio.h>
  char p_text[13] = "";
	sprintf(p_text,"LCD ID: %04X",lcd_id);	//LCD ID			 	
	LCD_DisplayStringLineCol(1, 2, p_text);		// line 0, column 2
}	   

uint16_t GRAM_mode;
/********************************************************************************************************
*  Function: ILI9328				                                                           
*  Object: lcd initialization
*  Input: none
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void ILI9328(void)
{
//	if(lcddev.id==0x9328)//ILI9328   OK  
  	LCD_WriteRegX(0x00EC,0x108F);// internal timing      
 		LCD_WriteRegX(0x00EF,0x1234);// ADD        
        LCD_WriteRegX(0x0001,0x0100);     
        LCD_WriteRegX(0x0002,0x0700);//電源開啟                    

	//DRIVE TABLE(寄存器 03H)
	/* ########################################
	Default XY: Portrait 1 (Logical (x, y) == Physical (X, Y))
	
 (Reset Key)     (USB Device) (On/Off switch)
 (0, 0) ---------- (219, 0)
        |   U    | 	
        |        | 	
   L    |        | 	R
        |        | 	
        |        | 	
        |   D    | 	
        ----------	(219, 319)
	
	         (STM32)
	//############################################
	*/
// bit: 5  4  3 
		//ID1/ID0 AM   FUNCTION
		//               x   y  (LCD coordinates)
		// 0  0   0	   R->L D->U (Portrait A)
		// 0  0   1	   D->U	R->L (Landscape A for bmp)
		// 0  1   0	   L->R D->U 
		// 0  1   1    D->U	L->R (Landscape A)
		// 1  0   0	   R->L U->D (Portrait A for bmp)
		// 1  0   1    U->D	R->L (Landscape B)
		// 1  1   0    L->R U->D (Portrait B; default XY coordinates)
		// 1  1   1	   U->D	L->R
  #if (LANDSCAPE == 1)
	// #### Landscape A ########
    // AM=1   (address is updated in vertical writing direction)    
		GRAM_mode = Bit(12)|(0x01<<4)| Bit(3); //BGR=1 (bit 12); I/D = 01b (bits 5:4) ; AM = 1 (bit 3)   
		LCD_WriteRegX(0x0003, GRAM_mode);
		// bit 12 (BGR) = 1: (Blue:Green:Red)
		// bit 3 (column and row exchange) = 1
		// bit 4 (X: column addr. order) = 1   rightward: 0 ==> width
    // bit 5 (Y: page (row) addr. order) = 0  upward: height ==> 0
		//
		//   --------------------> page (x)
		//   |
		//   |
		//   \/
		//   column (y) 
		//
  #else
    // AM=0   (address is updated in horizontal writing direction)            
//		LCD_WriteData(0xC8);	// bit 7, 6, 3 = 1  
	// #### Portait B ########
//		GRAM_mode = Bit(12)|(0x03<<4)|(0<<3); //BGR=1 (bit 12); I/D = 11b (bits 4:5) ; AM = 0 (bit 3)   
	// #### Portait A ########
		GRAM_mode = Bit(12)|(0x00<<4)|(0<<3); //BGR=1 (bit 12); I/D = 00b (bits 4:5) ; AM = 0 (bit 3)   
		LCD_WriteRegX(0x0003, GRAM_mode);
		// bit 12 (BGR) = 1
		// bit 4 (column addr. order) = 1   upward: width ==> 0
    // bit 5 (page (row) addr. order) = 1  rightward: height ==> 0
		// bit 3 (column and row exchange) = 0 (no)
		// bit 4 (X: column addr. order) = 1   rightward: 0 ==> width 
    // bit 5 (Y: page (row) addr. order) = 1  downward: 0 ==> height
		//
		//   column (x) 
		//   ^
		//   |
		//   |
		//   --------------------> page (y)
		//
  #endif
				LCD_WriteRegX(0x0004,0x0000);                                   
        LCD_WriteRegX(0x0008,0x0202);	           
        LCD_WriteRegX(0x0009,0x0000);         
        LCD_WriteRegX(0x000a,0x0000);//display setting         
        LCD_WriteRegX(0x000c,0x0001);//display setting          
        LCD_WriteRegX(0x000d,0x0000);//0f3c          
        LCD_WriteRegX(0x000f,0x0000);
		//電源配置
        LCD_WriteRegX(0x0010,0x0000);   
        LCD_WriteRegX(0x0011,0x0007);
        LCD_WriteRegX(0x0012,0x0000);                                                                 
        LCD_WriteRegX(0x0013,0x0000);                 
				// ==> LCD_DisplayOff(); // LCD_WriteRegX(0x0007,0x0001);
//When the display is turned off by setting D[1:0] = 01, the ILI9328 continues internal display operation. 		
				LCD_WriteRegX(0x0007,0x0001);  // D[1:0] = 01b              
       	delay_ms(50); 
        LCD_WriteRegX(0x0010,0x1490);   
        LCD_WriteRegX(0x0011,0x0227);
        delay_ms(50); 
        LCD_WriteRegX(0x0012,0x008A);                  
        delay_ms(50); 
        LCD_WriteRegX(0x0013,0x1a00);   
        LCD_WriteRegX(0x0029,0x0006);
        LCD_WriteRegX(0x002b,0x000d);
        delay_ms(50); 
        LCD_WriteRegX(0x0020,0x0000);   // Cursor X = 0                                                         
        LCD_WriteRegX(0x0021,0x0000);   // Cursor Y = 0 
		delay_ms(50); 
		//伽馬校正
        LCD_WriteRegX(0x0030,0x0000); 
        LCD_WriteRegX(0x0031,0x0604);   
        LCD_WriteRegX(0x0032,0x0305);
        LCD_WriteRegX(0x0035,0x0000);
        LCD_WriteRegX(0x0036,0x0C09); 
        LCD_WriteRegX(0x0037,0x0204);
        LCD_WriteRegX(0x0038,0x0301);        
        LCD_WriteRegX(0x0039,0x0707);     
        LCD_WriteRegX(0x003c,0x0000);
        LCD_WriteRegX(0x003d,0x0a0a);
        delay_ms(50); 
        LCD_WriteRegX(0x0050,0x0000); //水準GRAM起始位置 
        LCD_WriteRegX(0x0051,0x00ef); //水準GRAM終止位置: 0xEF = 239                  
        LCD_WriteRegX(0x0052,0x0000); //垂直GRAM起始位置                    
        LCD_WriteRegX(0x0053,0x013f); //垂直GRAM終止位置: 0x013F = 319  
 
        LCD_WriteRegX(0x0060,0xa700);        
				LCD_WriteRegX(0x0061,0x0001); // REV = 1 (bit 0): No inversion
        LCD_WriteRegX(0x006a,0x0000);
        LCD_WriteRegX(0x0080,0x0000);
        LCD_WriteRegX(0x0081,0x0000);
        LCD_WriteRegX(0x0082,0x0000);
        LCD_WriteRegX(0x0083,0x0000);
        LCD_WriteRegX(0x0084,0x0000);
        LCD_WriteRegX(0x0085,0x0000);
      
        LCD_WriteRegX(0x0090,0x0010);     
        LCD_WriteRegX(0x0092,0x0600);  
        //開啟顯示設定    
				// ==> LCD_DisplayOn();
        LCD_WriteRegX(0x0007,0x0133); // Normal Display: BASEE=1; GON=1; DTE=1; D[1:0] = 11b
}
/********************************************************************************************************
*  Function: LCD_Init				                                                           
*  Object: lcd initialization
*  Input: none
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_Init(void)
{ 
	volatile uint16_t data1;
		//-- LCD PORT INIT --
//		LCD_PortInit();
		GPIO_backlight = GPIO_outPPhigh(PNr_backlight, PIN_bl);	// backlight pin (LIG): PB0
		GPIO_reset = GPIO_outPPhigh(PNr_reset, PIN_rs);  				// reset pin (RST): PC5
		if (PinBit_FSMC_set1)	GPIO_AF12PPhigh(PNr_FSMC_set1, PinBit_FSMC_set1);
		if (PinBit_FSMC_set2)	GPIO_AF12PPhigh(PNr_FSMC_set2, PinBit_FSMC_set2);
		if (PinBit_FSMC_set3)	GPIO_AF12PPhigh(PNr_FSMC_set3, PinBit_FSMC_set3);
		if (PinBit_FSMC_set4)	GPIO_AF12PPhigh(PNr_FSMC_set4, PinBit_FSMC_set4);
/*  GPIOD: AF_FSMC, Speed_100MHz , push pull, no pull-up or pull-down 
	  --------------------------------------------------------------
	  | Pin: PD |   0   1    4    5       8    9   10   14   15
    | GLCD    |  D2  D3   (RD) (WR)    D13  D14  D15  D0   D1
	  --------------------------------------------------------------
    GPIOE: AF_FSMC, Speed_100MHz , push pull, no pull-up or pull-down 
	  --------------------------------------------------------------
	  | Pin: PE |    7   8   9   10  11  12  13   14   15
    | GLCD    |   D4  D5  D6  D7  D8  D9  D10  D11  D12
	  --------------------------------------------------------------
   GPIOG: AF_FSMC, Speed_100MHz , push pull, no pull-up or pull-down 
	  --------------------------------------------------------------
	  | Pin : PG |   0         12
    | GLCD     |  (RS)      (CS)
	            (FSMC_A10)  (FSMC_NE4)
	  --------------------------------------------------------------
*/

	
		LCD_FSMCConfig();

		//-- LCD RESET--
		LCD_Reset();
	// ######### Read ID, just for test ##################
		LCD_WriteReg(0x00);	// 0x00: get Chip Driver ID Code
 		data1 = LCD_ReadData;				// driver ID (0x9328)
		//-------------- Initial Sequence ---------------
		//************* Start Initial Sequence **********//	
		if(data1==0x9328)//ILI9328   OK  
		{
			ILI9328();
			lcd_id = data1;
		} else
		{
			while(1);
		}

	//----- setting the following initial variables:
	//----- Current font
	//----- wf, hf, bytes_wf, bytes_f
	LCD_SetFont( &LCD_DEFAULT_FONT);
	//----- text color; background color
	LCD_SetColors(Default_TextColor, Default_BackColor);
		
		LCD_Clear(Default_BackColor);
    delay_ms(50); 
#if Show_DeviceID
		LCD_DisplayDevID();
#endif
		LCD_DisplayOn();
}

/********************************************************************************************************
*  Function: LCD_OpenWin		(ILI9328)		                                                           
*  Object: lcd open window for display
*  Input: x0,y0, x1, y1
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_OpenWin(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint16_t cX, cY;
	
	if (x0 > x1) x1 = x0;
	else if (x1 > LCD_PIXEL_WIDTH) x1 = LCD_PIXEL_WIDTH; 
	if (y0 > y1) y1 = y0;
	else if (y1 > LCD_PIXEL_HEIGHT) y1 = LCD_PIXEL_HEIGHT; 
#if (LANDSCAPE == 1)
	// ########## For Landscape A #############
	// logical x (--> rightward increase): Physical Y  (<--- rightward decrease: 0 to 319)
	// logical y (downward increase): Physical X  (downward increase: 0 to 239)
	uint16_t tmp;

		tmp = x0;
		x0 = y0;		// i.e., X0
		y0 = LCD_PIXEL_WIDTH - x1;	// i.e., Y0	
		x1 = y1;		// i.e., X1
		y1 = LCD_PIXEL_WIDTH - tmp;	// i.e., Y1	
		cX = x0;		// x0 at X0
		cY = y1;		// y0 at Y1
#else
	// ########## For Portrait A #############
	// logical x (--> rightward increase): Physical X  (---> rightward decrease: 239 to 0)
	// logical y (downward increase): Physical Y  (downward decrease: 319 to 0)
	uint16_t tmp;
		tmp = x0;
		x0 = LCD_PIXEL_WIDTH - x1;	// i.e., X0	
		x1 = LCD_PIXEL_WIDTH - tmp;	// i.e., X1	
		tmp = y0;
		y0 = LCD_PIXEL_HEIGHT - y1;	// i.e., Y0	
		y1 = LCD_PIXEL_HEIGHT - tmp;	// i.e., Y1	
		cX = x1;		// x0 at X1
		cY = y1;	  // y0 at Y1
#endif
	
		LCD_WriteReg(0x50);					// X-start Set (50h)
		LCD_WriteData(x0);
		LCD_WriteReg(0x51);					// X-end Set (51h)
		LCD_WriteData(x1);		
		LCD_WriteReg(0x52);				// Y-start Set (52h) 
		LCD_WriteData(y0);
		LCD_WriteReg(0x53);				// Y-end Set (53h) 
		LCD_WriteData(y1);

	// Set start cursor point
		LCD_WriteReg(0x20);				// set cursor X 
		LCD_WriteData(cX);
		LCD_WriteReg(0x21);				// set cursor Y 
		LCD_WriteData(cY);

		LCD_WriteReg(0x22);				// Write Data to GRAM (R22h) 
}

/********************************************************************************************************
*  Function: LCD_toPortrait			(ILI9328)		                                                           
*  Object: lcd display in Portrait mode
*  Input: none
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_toPortrait(void)
{
	// #### Portait 1 ########
//		LCD_WriteRegX(0x0003, (Bit(12)|(0x03<<4)|(0<<3)));
	// #### Portait 0 ########
		LCD_WriteRegX(0x0003, (Bit(12)|(0x00<<4)|(0<<3)));
}
/********************************************************************************************************
*  Function: LCD_toLandscape			(ILI9328)		                                                           
*  Object: lcd display in Portrait mode
*  Input: none
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_toLandscape(void)
{
		LCD_WriteRegX(0x0003, (Bit(12)|(0x01<<4)| Bit(3)));
}
/********************************************************************************************************
*  Function: LCD_toDefaultDisDirection		(ILI9328)			                                                           
*  Object: lcd display in Portrait mode
*  Input: none
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_toDefaultDisDirection(void)
{
		LCD_WriteRegX(0x0003, GRAM_mode);
}

/********************************************************************************************************
*  Function: LCD_Clear				                                                           
*  Object: lcd clear screen
*  Input: backcolor
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_Clear(uint16_t BackColor)
{
		uint16_t i,j;
		LCD_OpenWin(0, 0, LCD_COL_NUM-1, LCD_ROW_NUM-1);
	
		for(i = 0; i < LCD_ROW_NUM; i++)
			 for(j =0; j < LCD_COL_NUM; j++)
					 LCD_WriteData(BackColor);
}

/**
  * @brief  Enable the Display. (ILI9328)
  * @param  None
  * @retval None
  */
void LCD_DisplayOn(void)
{
 	//---- turn on backlight ----------
	LCD_turnON_backlight;			// //   GPIOB->BSRR = 0x01;
	
  /* Display On */
  LCD_WriteRegX(0x0007,0x0133); // Normal Display: BASEE=1; GON=1; DTE=1; D[1:0] = 11b
}

/**
  * @brief  Disable the Display. (ILI9328)
  * @param  None
  * @retval None
  */
void LCD_DisplayOff(void)
{
  /* Display Off */
	LCD_WriteRegX(0x0007,0x0001);   // D[1:0] = 01b
//When the display is turned off by setting D[1:0] = 01, the ILI9328 continues internal display operation. 		
	//---- turn off backlight ----------
	LCD_turnOFF_backlight;		//   GPIOB->BSRR = 0x01<<16;	
}

//###########################################################
//
//     Hardware Abstract Layer
//
//###########################################################
/**
  * @brief  Sets a display window by width (w) and height (h)
  *
*/
#define LCD_DisplayWindow_WnH(x, y, w, h)  LCD_OpenWin(x, y, x+w-1, y+h-1)


/**
  * @brief  Sets the LCD Text and Background colors.
  * @param  _TextColor: specifies the Text Color.
  * @param  _BackColor: specifies the Background Color.
  * @retval None
  */
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor)
{
  DrawProp.TextColor = _TextColor;
  DrawProp.BackColor = _BackColor;
}

/**
  * @brief  Gets the LCD Text and Background colors.
  * @param  _TextColor: pointer to the variable that will contain the Text
            Color.
  * @param  _BackColor: pointer to the variable that will contain the Background
            Color.
  * @retval None
  */
void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor)
{
  *_TextColor = DrawProp.TextColor; 
	*_BackColor = DrawProp.BackColor;
}

/**
  * @brief  Sets the Text color.
  * @param  Color: specifies the Text color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetTextColor(__IO uint16_t Color)
{
  DrawProp.TextColor = Color;
}


/**
  * @brief  Sets the Background color.
  * @param  Color: specifies the Background color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetBackColor(__IO uint16_t Color)
{
  DrawProp.BackColor = Color;
}

/**
  * @brief  Sets the Text Font.
  * @param  fonts: specifies the font to be used.
  * @retval None
  */
void LCD_SetFont(sFONT *pfonts)
{
	DrawProp.pFont	= pfonts;	// pLCD_Currentfonts = pfonts;
	//----- modified by Shir-Kuan Lin
	wf = pfonts->Width; 	// font_width
	hf = pfonts->Height; // font_height
	bytes_wf = (wf+7) /8;	// number of bytes per width line in a font
	bytes_f = hf * bytes_wf;	// number of bytes per font (see Font12_Table or Font20_Table)
}

/**
  * @brief  Gets the Text Font.
  * @param  None.
  * @retval the used font.
  */
sFONT *LCD_GetFont(void)
{
  return pLCD_Currentfonts;
}

/**
  * @brief  Clears the selected line.
  * @param  Line: the Line to be cleared.
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..n
  * @retval None
  */
void LCD_ClearStringLine(uint16_t LineNr)
{
  uint16_t bColor;
  uint16_t x, y;
	if (((LineNr+1) * pLCD_Currentfonts->Height) > (LCD_PIXEL_HEIGHT)){
		return;
	}
	LineNr = LineNr * pLCD_Currentfonts->Height;
	bColor = DrawProp.BackColor;
	
	LCD_OpenWin(0, LineNr, LCD_PIXEL_WIDTH, LineNr+hf-1);
  for(y = 0; y < hf; y++)		
  {
		for(x = 0; x <= LCD_PIXEL_WIDTH; x++)
		{
			LCD_WriteData(bColor);
		}
	}
}

/********************************************************************************************************
*  Function: LCD_DisplayChar				                                                           
*  Object: Display an ASCII character
*  Input: start point, end point, ASCII code, text color, background color
*  Output: none                                  
*  brief: none
********************************************************************************************************/
static void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos,  uint8_t Ascii, uint16_t fColor, uint16_t bColor)
{
	// wf =: font_width,
	// hf =:  font_height
// bytes_wf = number of bytes per width line in a font
// bytes_f  = number of bytes per font

	int	iw;
  uint32_t byteNr, jw;
	uint16_t  hp_limit;
	const uint8_t *c;
	
	if((Ascii > '~') || (Ascii < ' ')) Ascii= ' ';
		
	iw = Ascii - 32;  // make ' ' (space) at offset of 0
  c = &pLCD_Currentfonts->table[iw * bytes_f];	
	
	LCD_OpenWin(Xpos, Ypos, Xpos+wf-1, Ypos+hf-1);
	byteNr =0;
	hp_limit = Ypos + hf;
	
  for(; Ypos < hp_limit; Ypos++)		// font height 
  {
		uint8_t as;

		as = c[byteNr++];
		iw = 0;
    for(jw = 0; jw < wf; jw++)	//  loop for the font width (wf)
    {
				if(as & (0x80))
				{
					LCD_WriteData(fColor);
				}
				else
				{
					LCD_WriteData(bColor);
				}
				as <<= 1;
				iw++;
				if (iw == 8){							// reach 1 byte = 8 bits
					as = c[byteNr++];
					iw = 0;
				}
    }
	}
}

/**
  * @brief  Displays characters between a line segment on the LCD.
  * @param  Xpos: X position (in pixel)
  * @param  Ypos: Y position (in pixel)   
  * @param  Text: Pointer to string to display on LCD
  * @retval None
  */
static void LCD_DisplayStringSegment(uint16_t Xpos, uint16_t Ypos, char *Text) //, uint16_t font_width)
{
	uint16_t fColor, bColor;
	
	// wf =: font_width,
	// hf =:  font_height
// 	font_width = pLCD_Currentfonts->Width  // wf = font_width
	fColor = DrawProp.TextColor;
	bColor = DrawProp.BackColor;
  while ((*Text != 0) & (Xpos  <= (LCD_PIXEL_WIDTH - (wf-1))) )
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Xpos, Ypos, (uint8_t)*Text, fColor, bColor);
    /* Decrement the column position by 16 */
    Xpos += wf;
    /* Point on the next character */
    Text++;
  }
}

/**
  * @brief  Displays characters on the LCD.
  * @param  Xpos: X position (in pixel)
  * @param  Ypos: Y position (in pixel)   
  * @param  Text: Pointer to string to display on LCD
  * @param  Mode: Display mode
  *          This parameter can be one of the following values:
  *            @arg  CENTER_MODE
  *            @arg  RIGHT_MODE
  *            @arg  LEFT_MODE   
  * @retval None
  */
void LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, char *Text, Line_ModeTypdef Mode)
{
  uint32_t leftcolumn=0;
  uint32_t size = 0, xBlank=0; 
  char  *ptr = Text;
  
	// wf =: font_width,
	// hf =:  font_height
// 	font_width = pLCD_Currentfonts->Width  // wf = font_width
	if (Mode == LEFT_MODE){
				leftcolumn = Xpos;				
	} else
	{
 /* Get the text size */
		while (*ptr++) size ++ ;
  
  /* Characters number per line */
		xBlank = ((LCD_PIXEL_WIDTH+1) /wf);		// maximum number of characters per line
		if (size > xBlank){
			xBlank = 0;
		}	else {		
			xBlank = (xBlank - size)* wf;			
		}
		xBlank += (LCD_PIXEL_WIDTH+1) %wf;		// width of blank space of this string line
	}
		switch (Mode)
		{
		case LEFT_MODE:
			{
				break;
			}
		case CENTER_MODE:
			{
				leftcolumn = Xpos + xBlank/ 2;
				break;
			}
		case RIGHT_MODE:
			{
				// if leftcolumn = xBlank, the right border of the string is at the right side of the screen
				leftcolumn = LCD_PIXEL_WIDTH - Xpos;
				if (leftcolumn > xBlank){
					leftcolumn = 0;
				} else {
					leftcolumn = xBlank - leftcolumn;
				}
				break;
			}
		}
	
  /* Send the string character by character on LCD */
	LCD_DisplayStringSegment((uint16_t) leftcolumn, Ypos, Text);
}


/**
  * @brief  Displays the string on the required row and column
  * @param ount: specifies the delay time length (time base 10 ms).
  * @retval : None
  */
void LCD_DisplayStringLineCol(uint8_t LineNr, uint16_t ColNr, char *ptr)
{
  uint16_t Xpos, Ypos ;
	
	// wf =: font_width,
	// hf =:  font_height
	Xpos = ColNr * wf;
	Ypos = LineNr * hf;
	
	LCD_DisplayStringSegment(Xpos, Ypos, ptr);
}

/**
  * @brief  Draws a pixel on LCD.
  * @param  Xpos: X position 
  * @param  Ypos: Y position
  * @param  Color: Pixel color in RGB mode (5-6-5)  
  * @retval None
  */
void LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t Color)
{
		LCD_OpenWin(Xpos, Ypos, Xpos, Ypos);
	  LCD_WriteData(Color);
}


/**
  * @brief  Draws a horizontal line on LCD.
  * @param  Xpos: X position 
  * @param  Ypos: Y position
  * @param  Length: line length
  * @param  RGB_Code: Pixel color in RGB mode (5-6-5)  
  * @retval None
  */
void LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
	uint16_t color=DrawProp.TextColor;
	uint16_t i, x1;

	if (Ypos > LCD_PIXEL_HEIGHT) return;
	x1 = Xpos+Length-1;
	if(x1 > LCD_PIXEL_WIDTH){
		x1 = LCD_PIXEL_WIDTH;
		Length = LCD_PIXEL_WIDTH - Xpos + 1;
	}
	LCD_OpenWin(Xpos, Ypos, x1, Ypos);
		for(i = 0; i < Length; i++)
			 LCD_WriteData(color);
}

/**
  * @brief  Draws a vertical line on LCD.
  * @param  Xcen: X position 
  * @param  Ypos: Y position
  * @param  Length: line length
  * @param  RGB_Code: Pixel color in RGB mode (5-6-5)  
  * @retval None
  */
void LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
	uint16_t color=DrawProp.TextColor;
	uint16_t j, y1;

	if (Xpos > LCD_PIXEL_WIDTH) return;
	y1 = Ypos+Length-1;
	if(y1 > LCD_PIXEL_HEIGHT){
		y1 = LCD_PIXEL_HEIGHT;
		Length = LCD_PIXEL_WIDTH - Xpos + 1;
	}
	LCD_OpenWin(Xpos, Ypos, Xpos, y1);
		for(j = 0; j < Length; j++)
			 LCD_WriteData(color);
}

/**
  * @brief  Draws a simple cross on LCD.
  * @param  Xcen: X center position 
  * @param  Ypos: Y center position
  * @param  Length: line length
  * @param  RGB_Code: Pixel color in RGB mode (5-6-5)  
  * @retval None
  */
void LCD_DrawSimpleCross(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
	uint16_t hL;
	if ((Length & Bit(0)) == 0)		//even value
			Length +=1;
	
	hL = Length/2;
	LCD_DrawVLine(Xpos, Ypos-hL, Length);
	LCD_DrawHLine(Xpos-hL, Ypos, Length);
}

/**
  * @brief  Displays a rectangle border.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: display rectangle height.
  * @param  Width: display rectangle width.
  * @retval None
  */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
	//===  vertical lines
  LCD_DrawVLine(Xpos, Ypos, Height);
  LCD_DrawVLine((Xpos + Width - 1), Ypos, Height);
	
	//===  horizontal lines
  LCD_DrawHLine(Xpos, Ypos, Width);
  LCD_DrawHLine(Xpos, (Ypos + Height-1), Width);
}

/**
  * @brief  Displays a circle border.
  * @param  Xcen: specifies the X position of the center.
  * @param  Ycen: specifies the Y position of the center.
  * @param  Radius
  * @retval None
  */
void LCD_DrawCircle(uint16_t Xcen, uint16_t Ycen, uint16_t Radius)
{
	uint16_t color=DrawProp.TextColor;

  int32_t  next;/* Decision Variable */
  uint16_t  Ri;/* increasing from 0 to Radius */
  uint16_t  Rd;/* decreasing from Radius to 0 */

  next = 3 - (Radius << 1);
  Ri = 0;
  Rd = Radius;
	
  while (Ri <= Rd)
  {
		int32_t x0, y0;

		x0 = Xcen -	Ri;
		y0 = Ycen -	Rd;
		if(x0<0) x0=0;
		if(y0<0) y0=0;
				LCD_DrawPixel((uint16_t)x0, (uint16_t)y0, color);                          
				LCD_DrawPixel((uint16_t)x0, Ycen+Rd, color);                  
				LCD_DrawPixel(Xcen+Ri, (uint16_t)y0, color);             
				LCD_DrawPixel(Xcen+Ri, Ycen+Rd, color);             
		x0 = Xcen -	Rd;
		y0 = Ycen -	Ri;
		if(x0<0) x0=0;
		if(y0<0) y0=0;
				LCD_DrawPixel((uint16_t)x0, (uint16_t)y0, color);              	         
				LCD_DrawPixel((uint16_t)x0, Ycen+Ri, color);             
				LCD_DrawPixel(Xcen+Rd, (uint16_t)y0, color);                      
				LCD_DrawPixel(Xcen+Rd, Ycen+Ri, color);                          

    if (next < 0)
    {
      next += (Ri << 2) + 6;
    }
    else
    {
      next += ((Ri - Rd) << 2) + 10;
      Rd--;
    }
    Ri++;
  }
}

/**
  * @brief  Displays a full rectangle with TestColor.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @retval None
  */
void LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
	uint16_t color=DrawProp.TextColor;
	uint16_t Xend = Xpos+ Width-1, Yend = Ypos + Height-1;
	uint16_t i, j;
	
		LCD_OpenWin(Xpos, Ypos, Xend, Yend);
		for(i = 0; i < Width; i++)
			 for(j = 0; j < Height; j++)
					 LCD_WriteData(color);
	
}

/**
  * @brief  Displays a full circle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius
  * @retval None
  */
void LCD_FillCircle(uint16_t Xcen, uint16_t Ycen, uint16_t Radius)
{
//	uint16_t color=DrawProp.TextColor;
  int32_t  next;/* Decision Variable */
  uint16_t  Ri;/* increasing from 0 to Radius */
  uint16_t  Rd;/* decreasing from Radius to 0 */

  next = 3 - (Radius << 1);
  Ri = 0;
  Rd = Radius;

  while (Ri <= Rd)
  {
		int32_t X0, Y0;

		X0 = Xcen - Ri;
		Y0 = Ycen -	Rd;
		if(X0<0) X0=0;
		if(Y0<0) Y0=0;
		LCD_DrawVLine((uint16_t)X0, (uint16_t)Y0, (uint16_t)(Ycen + Rd - Y0));				// vertical line in landscape  view
		X0 = Xcen + Ri;
		LCD_DrawVLine((uint16_t)X0, (uint16_t)Y0, (uint16_t)(Ycen + Rd - Y0));				// vertical line in landscape  view
		X0 = Xcen - Rd;
		Y0 = Ycen -	Ri;
		if(X0<0) X0=0;
		if(Y0<0) Y0=0;
		LCD_DrawHLine((uint16_t)X0, (uint16_t)Y0, (uint16_t)(Xcen + Rd - X0));				// horizontal line in landscape  view
		Y0 = Ycen +	Ri;
		LCD_DrawHLine((uint16_t)X0, (uint16_t)Y0, (uint16_t)(Xcen + Rd - X0));				// horizontal line in landscape  view
    if (next < 0)
    {
      next += (Ri << 2) + 6;
    }
    else
    {
      next += ((Ri - Rd) << 2) + 10;
      Rd--;
    }
    Ri++;
  }
}

/**
  * @brief  Displays an uni-line (between two points).
  * @param  x1: specifies the point 1 x position.
  * @param  y1: specifies the point 1 y position.
  * @param  x2: specifies the point 2 x position.
  * @param  y2: specifies the point 2 y position.
  * @retval None
  */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t color=DrawProp.TextColor;
  int16_t deltax, deltay, x, y, xinc1, xinc2,
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
  curpixel = 0;

  deltax = (int16_t)ABS(x2 - x1);        /* The difference between the x's */
  deltay = (int16_t)ABS(y2 - y1);        /* The difference between the y's */
  x = (int16_t)x1;                       /* Start x off at the first pixel */
  y = (int16_t)y1;                       /* Start y off at the first pixel */
	
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
		if(x < 0 || x > LCD_PIXEL_WIDTH || y < 0 || y > LCD_PIXEL_HEIGHT)
		{
			goto escape;
		}
		LCD_DrawPixel((uint16_t)x, (uint16_t)y, color);
escape:
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}

/**
  * @brief  Displays a mono-color picture.
  * @param  Pict: pointer to the picture array.
  * @retval None
  */
void LCD_DrawMonoPict(const uint32_t *Pict)
{
	uint16_t fcolor=DrawProp.TextColor;
	uint16_t bcolor=DrawProp.BackColor;
  uint32_t index = 0, i = 0;

  LCD_OpenWin(0, 0, LCD_PIXEL_WIDTH, LCD_PIXEL_HEIGHT);
//  LCD_SetCursor(0, LCD_PIXEL_WIDTH );
//  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
 
	for(index = 0; index < 2400; index++)
  {
    for(i = 0; i < 32; i++)						// 2400 x 32 = 240 x 320
    {
      if((Pict[index] & (1 << i)) == 0x00)
      {
        LCD_WriteData(bcolor);		// background color
      }
      else
      {
        LCD_WriteData(fcolor);
      }
    }
  }
}

/**
  * @brief  Draws a bitmap picture (16 bpp) (24 bpp will be transfered to 16 bpp automatically).
  * @param  Xpos: Bmp X position in the LCD
  * @param  Ypos: Bmp Y position in the LCD
  * @param  pbmp: Pointer to Bmp picture address.
  * @retval None
  */
void LCD_DrawBitmap(uint16_t x0, uint16_t y0, uint8_t *BmpAddress)	
{
  uint32_t index;
  uint16_t bitCount;
  uint16_t X0, Y0;
	
	uint32_t height, i, n;
  uint32_t width, j, j_ignore;
  
  //---------------->>>>>>>>>>>>>>>
	/* Read BMP signature */
  width = *(uint16_t *) (BmpAddress + 0);		// get 2-Bytes value
	if (width != 0x4D42) return;
	
  /* Read bitmap width */
  width = *(uint16_t *) (BmpAddress + 18);
  width |= (uint32_t) (*(uint16_t *) (BmpAddress + 20)) << 16;
  
  /* Read bitmap height */
  height = *(uint16_t *) (BmpAddress + 22);
  height |= (uint32_t) (*(uint16_t *) (BmpAddress + 24)) << 16; 

	//-----------------<<<<<<<<<<<<<<<<<<<<<
  
  /* Read number of bits per pixel */
  bitCount = *(uint16_t *) (BmpAddress + 28);

  /* Read bitmap size */
  /* Get bitmap data address offset */
  index = *(__IO uint16_t *) (BmpAddress + 10);
  index |= (uint32_t) (*(__IO uint16_t *) (BmpAddress + 12)) << 16;
  BmpAddress += index;
	
//>>>######################################################
  /* Set GRAM write direction and BGR = 1 (ILI9328) */
	// ########## For Landscape A #############
// bit: 5  4  3 
		//ID1/ID0 AM   FUNCTION
		// 0  0   1	   D->U	(R->L)
		LCD_WriteReg(0x0003);  // GRAM Mode (03h)
  #if (LANDSCAPE == 1)
	  uint16_t tmp;
		tmp = GRAM_mode & ~(Bit(4)|Bit(5));			// I/D = 00b
    /* BMP updates from bottom to top and left to right        */
		LCD_WriteData(tmp);	
		//   column (y) 
		// L   ^
		//     |
		//     |
		//     |(x0,y1)  
		// R,D  --------------------> U, page (x)
		// 
		// (x0,y1) ==>  X0 = y1; Y0 = LCD_PIXEL_WIDTH - x0
  #else
	// ########## For Portrait A #############
// bit: 5  4  3 
		//ID1/ID0 AM   FUNCTION
		// 1  0   0	   R->L (U->D)
	  uint16_t tmp;
		tmp = (GRAM_mode & ~(Bit(4)|Bit(5)) ) | Bit(5);			// I/D = 10b
		LCD_WriteData(tmp);
		//   column (x) 
		// L   ^
		//     |
		//     |                   (x0,y1)
		// R,D  <-------------------- U, page (y)
		//
		// (x0,y1) ==>  X0 = LCD_PIXEL_WIDTH - x0; Y0 = LCD_PIXEL_HEIGHT - y1
  #endif
//<<<######################################
//>>>>>########### revised on 2024.08.01 ###############################
//	LCD_DisplayWindow_WnH(Xpos, Ypos, width, height);
	i = y0 + height - 1;			// y1
	n = 0;
	if (i > LCD_PIXEL_HEIGHT){
		n = i - LCD_PIXEL_HEIGHT;			// ignore the image part on the last "n" lines
		i = LCD_PIXEL_HEIGHT;			// y1
	}
	j = x0 + width - 1;			// x1
	j_ignore = width;
	if (j > LCD_PIXEL_WIDTH){
		if (x0 > LCD_PIXEL_WIDTH) j_ignore = 0;
		else j_ignore = (LCD_PIXEL_WIDTH+1) - x0;
		j = LCD_PIXEL_WIDTH;		// x1
	}
	
	LCD_OpenWin(x0, y0, (uint16_t)j, (uint16_t)i);
	
#if (LANDSCAPE == 1)
		X0 = (uint16_t)i; 	// i = y1;
		Y0 = LCD_PIXEL_WIDTH - x0;
#else
		X0 = LCD_PIXEL_WIDTH - x0;
		Y0 = LCD_PIXEL_HEIGHT - i; 	// i = y1;
#endif
		LCD_WriteReg(0x20);				// set cursor X 
		LCD_WriteData(X0);
		LCD_WriteReg(0x21);				// set cursor Y 
		LCD_WriteData(Y0);
		LCD_WriteReg(0x22);				// Write Data to GRAM (R22h) 
	
//<<<<<########### revised on 2024.08.01 ###############################

  if (bitCount == 24)
	{
		uint8_t skip;
		uint16_t n_ignore = (uint16_t) (width - j_ignore);		//#### revised on 2024.08.01
		
		skip = 0x03 & (4 - ( (width*3) &0x03));   // align in 4 multiple
		if (n > 0){																// ignore "n" lines
			BmpAddress += n * (3*width + skip);					//#### revised on 2024.08.01
		}
			
		for (i = n; i<height; i++)				
		{ 
			for (j=0; j<j_ignore; j++)	//#### revised on 2024.08.01
			{
				uint16_t temp;
/*				uint16_t t2;
				t2 = (*(__IO uint8_t *)BmpAddress++);
				temp = t2 >>3;
				t2 = (*(__IO uint8_t *)(BmpAddress++));
				temp |= (t2 >>3)<<6;
				t2 = (*(__IO uint8_t *)(BmpAddress++));
				temp |= (t2 >>3)<<(5+6);
*/
					temp = (*(__IO uint8_t *)BmpAddress++)>>3;
					temp |= ((*(__IO uint8_t *)(BmpAddress++) )>> 3)  << 6;
					temp |= ((*(__IO uint8_t *) (BmpAddress++) )>> 3)  << (6+5);
					LCD_WriteData(temp);
			}
			BmpAddress += 3*n_ignore;					//#### revised on 2024.08.01
			BmpAddress += skip;
		}
	}
	else // bitCount == 16 [RED (5bits): GREEN (6bits): BLUE (5bits)]
	{
//>>>>>########### revised on 2024.08.01 ###############################
		uint16_t n_ignore = (uint16_t) (width - j_ignore);		

		if (n > 0){				// ignore "n" lines		
			BmpAddress += n * (2*width);
		}
		
		for (i = n; i<height; i++)				
		{ 
			for (j=0; j<j_ignore; j++)	//#### revised on 2024.08.01
			{
				LCD_WriteData(*(__IO uint16_t *)BmpAddress);
				BmpAddress += 2;
			}
			BmpAddress += 2 * n_ignore;					//#### revised on 2024.08.01
		}
//<<<<<########### revised on 2024.08.01 ###############################
  }

//>>>######################################################
  /* Set GRAM write direction and BGR = 1 (ILI9428) */
		LCD_WriteRegX(0x0003, GRAM_mode);  // GRAM Mode 
//<<<######################################
}

/**
  * @brief  Draws RGB Image (16 bpp).
  * @param  Xpos:  X position in the LCD
  * @param  Ypos:  Y position in the LCD
  * @param  Xsize: X size in the LCD
  * @param  Ysize: Y size in the LCD
  * @param  pdata: Pointer to the RGB Image address.
  * @retval None
  */
void LCD_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata)
{
  uint16_t x1, y1;
  uint16_t i, j;
  
	if ( (Xpos > LCD_PIXEL_WIDTH) || (Ypos > LCD_PIXEL_HEIGHT)) return;
	x1 = Xpos + Xsize-1;
	y1 = Ypos + Ysize-1;
	if (x1 > LCD_PIXEL_WIDTH) Xsize = LCD_PIXEL_WIDTH+1 - Xpos;
	if (y1 > LCD_PIXEL_HEIGHT) Ysize = LCD_PIXEL_HEIGHT+1 - Ypos;
  
  LCD_DisplayWindow_WnH(Xpos, Ypos, Xsize, Ysize);
  
  for(j= 0; j < Ysize; j++)
  {
		for(i= 0; i < Xsize; i++)
		{
    /* Write 16-bit GRAM Reg */
			LCD_WriteData(*(volatile uint16_t *)pdata);
			pdata += 2;
		}
	}
}


/**
  * @brief  Displays an poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLine(pPoint Points, uint16_t PointCount)
{
  uint16_t X, Y;

  if(PointCount < 2)
  {
    return;
  }

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    LCD_DrawLine(X, Y, Points->X, Points->Y);
  }
}

/**
  * @brief  Displays an relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @param  Closed: specifies if the draw is closed or not.
  *           1: closed, 0 : not closed.
  * @retval None
  */
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed)
{
  uint16_t X = 0, Y = 0;
  pPoint First = Points;

  if(PointCount < 2)
  {
    return;
  }
  X = Points->X;
  Y = Points->Y;
  while(--PointCount)
  {
    Points++;
    LCD_DrawLine(X, Y, X + Points->X, Y + Points->Y);
    X = X + Points->X;
    Y = Y + Points->Y;
  }
  if(Closed)
  {
    LCD_DrawLine(First->X, First->Y, X, Y);
  }
}

/**
  * @brief  Displays a closed poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_DrawPolygon(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLine(Points, PointCount);
  LCD_DrawLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);
}

/**
  * @brief  Displays a relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLineRelative(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLineRelativeClosed(Points, PointCount, 0);
}

/**
  * @brief  Displays a closed relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLineRelativeClosed(Points, PointCount, 1);
}

/**
  * @brief  Draws an ellipse on LCD.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius
  * @retval None
  */
void LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K, x_f;
  
  K = ((float) YRadius)/((float) XRadius);		// K = YR/XR
  
  do { 
		x_f = ((float) x)/K;	
    LCD_DrawPixel((uint16_t) (Xpos-(uint16_t)x_f), (uint16_t) (Ypos+y), DrawProp.TextColor);
    LCD_DrawPixel((uint16_t) (Xpos+(uint16_t)x_f), (uint16_t) (Ypos+y), DrawProp.TextColor);
    LCD_DrawPixel((uint16_t) (Xpos+(uint16_t)x_f), (uint16_t) (Ypos-y), DrawProp.TextColor);
    LCD_DrawPixel((uint16_t) (Xpos-(uint16_t)x_f), (uint16_t) (Ypos-y), DrawProp.TextColor);      
    
    e2 = err;
    if (e2 <= x) {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;     
  }
  while (y <= 0);
}


/**
  * @brief  Fills a triangle (between 3 points).
  * @param  Points: Pointer to the points array
  * @param  Points[0].x: Point 1 X position
  * @param  Points[0].y: Point 1 Y position
  * @param  Points[1].x: Point 2 X position
  * @param  Points[1].y: Point 2 Y position
  * @param  Points[2].x: Point 3 X position
  * @param  Points[2].y: Point 3 Y position
  * @retval None
  */
// revised on 2025/2/12
void LCD_FillTriangle(pPoint Points)
{
	uint16_t	xp[3], yp[3], i, tmp, x_mid;
  int32_t int_tmp, deltaX1, deltaY1, deltaX2, deltaY2;
  uint16_t x, y, ii;
	
		xp[0] = Points[0].X;
		yp[0] = Points[0].Y;
	for (i=1; i<3; i++)
	{
		x = Points[i].X;
		y = Points[i].Y;
		  if (yp[0] > y){
				xp[i] = xp[0];
				xp[0] = x;
				yp[i] = yp[0];
				yp[0] = y;
			} else{
				xp[i] = x;
				yp[i] = y;
			}
	}	
	if (yp[1] > yp[2]){
				tmp = xp[1];
				xp[1] = xp[2];
				xp[2] = tmp;
				tmp = yp[1];
				yp[1] = yp[2];
				yp[2] = tmp;
	}
	// NOTE: yp[2] >= yp[1] >= yp[0]
	//----------
	// Point 0 with smallest y; 
	// Point 1 at the middle height
	// Point 2 with largest y
	//------------ 
	deltaY2 = (int32_t) yp[2]- yp[0];		// deltaY2 >= 0
		if (deltaY2 == 0){	// i.e. deltaY2 = deltaY1 = 0
			if( xp[2] < xp[0]){ 
				x = xp[2];
				tmp = xp[0] - x;
			} else {
				x = xp[0];
				tmp = xp[2] - x;
			}
			LCD_DrawHLine(x, yp[0], tmp+1);	// a horizontal line
			return;
		}
	// ---------- deltaY2 > 0 in the following --------------------
  deltaX2 = (int32_t) xp[2]- xp[0];		// Line 0_2
	deltaY1 = (int32_t) yp[1]- yp[0];		// deltaY1 >= 0
  deltaX1 = (int32_t) xp[1]- xp[0];		// Line 0_1
		int_tmp = (deltaX2 * deltaY1) /deltaY2;

	x_mid = (uint16_t)(xp[0] + int_tmp);
	//====== 1st subtriangle (P0-P1-Px, where Px=(int_temp, y1))
		if ( x_mid < xp[1] ){ // Line 0_2 is left to Line 0_1
			int_tmp = deltaX1;
			deltaX1 = deltaX2;				// Line 0_2
			deltaX2 = int_tmp;				// Line 0_1
			int_tmp = deltaY1;
			deltaY1 = deltaY2;
			deltaY2 = int_tmp;
		}
	x = xp[0];
	y = yp[0];
	if (deltaY1 == 0){
		if (xp[1] < x) {
			x = xp[1];
			tmp = (uint16_t) (-deltaX1);
		}	else {
		  tmp = (uint16_t) deltaX1;
		}
		LCD_DrawHLine(x, y, tmp+1);	// a horizontal line
	}else {
		LCD_DrawPixel(x, y, DrawProp.TextColor); // draw only point P0
		ii = 0;
		for (i=y+1; i<=yp[1]; i++)	// downward
		{
				int32_t x_tmp, length;
				
				ii++;
				x_tmp = deltaX1*ii /deltaY1;
				 length = deltaX2*ii /deltaY2- x_tmp; // Note: deltaY2 >= deltaY1 > 0 and  ll must >= 0
				 LCD_DrawHLine((uint16_t) (x+x_tmp), i, (uint16_t) length+1);
		}	
	}
	//====== 2nd subtriangle 
	
	if ( x_mid < xp[1] ){ // Line 2_0 is left to Line 2_1
		deltaX1 = (int32_t) -deltaX1;		// line 2_0
//		deltaY1 = (int32_t) yp[2]- yp[0];		
		deltaX2 = (int32_t) xp[1]-xp[2];		// line 2_1
		deltaY2 = (int32_t) yp[2]- yp[1];		
	} else{	// for x_mid >= xp[1]; Line 2_0 is right to Line 2_1
		deltaX1 = (int32_t) xp[1]-xp[2];		// line 2_1
		deltaY1 = (int32_t) yp[2]- yp[1];		
		deltaX2 = (int32_t) -deltaX2;		// line 2_0
//		deltaY2 = (int32_t) yp[2]- yp[0];		
	}
		LCD_DrawPixel(xp[2], yp[2], DrawProp.TextColor); // draw only point P0
		ii = 0;
		for (i=yp[2]-1; i>=yp[1]; i--)			// upward
		{
				int32_t x_tmp, length;
				
				ii++;
				x_tmp = deltaX1*ii / deltaY1;
				length = deltaX2*ii /deltaY2- x_tmp; // Note: deltaX2 >= deltaX1 > 0, so length must >= 0

				LCD_DrawHLine((uint16_t) (x+x_tmp), i, (uint16_t) length+1);
		}	
	}

/**
  * @brief  Draws a full poly-line (between many points).
  * @param  Points: Pointer to the points array
  * @param  PointCount: Number of points
  * @retval None
  */
void LCD_FillPolygon(pPoint Points, uint16_t PointCount)
{
  Point tmp_pt[3];
  int16_t i;

  if(PointCount <=2)
  {
    LCD_PolyLine(Points, PointCount);
		return;
  }

	LCD_FillTriangle(Points);
	if (PointCount == 3){
		return;
	}
  

	tmp_pt[0].X = Points[0].X;
	tmp_pt[0].Y = Points[0].Y;
	
  for(i = 1; i < (PointCount-1); i++)
  {
		tmp_pt[1].X = Points[i+1].X;
		tmp_pt[1].Y = Points[i+1].Y;
		tmp_pt[2].X = Points[i+2].X;
		tmp_pt[2].Y = Points[i+2].Y;
    
		LCD_FillTriangle(tmp_pt);
  }
}

/**
  * @brief  Draws a full ellipse.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius  
  * @retval None
  */
void LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K, x_f;
  
  K = ((float) YRadius)/((float) XRadius);		// K = YR/XR
  
  do 
  { 
    x_f = ((float) x)/K;	
		LCD_DrawHLine((uint16_t) (Xpos-(uint16_t)x_f), (uint16_t) (Ypos+y), ((uint16_t)(2*x_f) + 1));
    LCD_DrawHLine((uint16_t) (Xpos-(uint16_t)x_f), (uint16_t) (Ypos-y), ((uint16_t)(2*x_f) + 1));
    
    e2 = err;
    if (e2 <= x) 
    {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}


//============== NEW by Shir-Kuan Lin ++++++++++++
sFONT *lFont;
uint16_t	ltColor, lbColor;

//----------------------
void LCD_SaveColors(void)
{
	ltColor = DrawProp.TextColor;
	lbColor = DrawProp.BackColor;
}
//----------------------
void LCD_RestoreColors(void)
{
	DrawProp.TextColor = ltColor;
	DrawProp.BackColor = lbColor;
}
//----------------------
void LCD_SaveFont(void)
{
	lFont = pLCD_Currentfonts;
}
//----------------------
void LCD_RestoreFont(void)
{
  LCD_SetFont(lFont);
}

//########################################################
//########################################################
//  Test  Experiments 													 
//########################################################
//########################################################

/**
  * @brief  Test LCD Display
  * @retval None
  */
void LCD_RGB_Test(void)
{
  uint32_t index, p1, p;

  LCD_OpenWin(0, 0, LCD_PIXEL_WIDTH, LCD_PIXEL_HEIGHT);
	p = (LCD_PIXEL_HEIGHT+1)/3;
	p = p * (LCD_PIXEL_WIDTH+1);

	/* R */
  for(index = 0; index <= p; index++)
  {
    LCD_WriteData(LCD_COLOR_RED);
  }
	  
  /* G */
	p = 2 * p;
  for(; index <= p; index++)
  {
    LCD_WriteData(LCD_COLOR_GREEN);
  }
	  
	/* B */
	p = (LCD_PIXEL_HEIGHT+1)*(LCD_PIXEL_WIDTH+1);
  for(; index <= p; index++)
  {
    LCD_WriteData(LCD_COLOR_BLUE);
  }

	p1 = (DrawProp.pFont)->Height; // font_height in pixel;	
	p1 = ((LCD_PIXEL_HEIGHT+1) +(p1-1)) /p1;
	LCD_SetColors(WHITE, RED);
  LCD_DisplayStringLineCol(2, 1,"R"); // line 2, column 1
	p = p1/3;
	LCD_SetBackColor(GREEN);
  LCD_DisplayStringLineCol((uint8_t) (2+p), 1,"G"); // line , column 1
	p = (2*p1)/3;
	LCD_SetBackColor(BLUE);
  LCD_DisplayStringLineCol((uint8_t) (2+p), 1,"B"); // line , column 1
	  delay_ms(1000); /* delay 1000 ms */
}


//==========================
// LCD display off/on test 
//==========================
void LCD_DisplayOnTest(void)
{
	LCD_DisplayOff();

	delay_ms(3000);
 	//---- turn on Display ----------
	LCD_DisplayOn();
}

/**
  * @brief  Menu Initialisation routine
  */
void MenuInit(void)
{
  uint16_t wd;
	LCD_RGB_Test();
	delay_ms(200);

	/*	save current Font, Back Color*/
	LCD_SaveColors();

	LCD_SaveFont();
//	LCD_SetFont(&Font20);
  LCD_Clear(LCD_COLOR_BLUE2);
	LCD_SetColors(LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2); // Test = white; back = blue

	LCD_DisplayStringLineCol(0, 2,"PARADIGM LCD DEMO");		// line 0, column 2
  LCD_SetTextColor(LCD_COLOR_GREEN);
  LCD_DisplayStringLineCol(2, 1,"Watch LEDs flashing"); // line 2, column 1

	//----------------------------------
	if (LANDSCAPE == 0) wd = 184;
	else	wd = 220;
  LCD_SetTextColor(LCD_COLOR_YELLOW);
  /*Draw a rectangle with: Start X-Cood,  Start Y-Cood,  Width,  Heigt*/
  LCD_DrawRect(50,80,wd,120);
  LCD_SetTextColor(LCD_COLOR_CYAN);
  /*Draw a rectangle with: Start X-Cood,  Start Y-Cood,  Width,  Heigt*/
  LCD_DrawRect(45,75,wd+10,130);
	//----------------------------------
  LCD_SetTextColor(LCD_COLOR_RED);
	LCD_FillRect(53, 83, wd-6, 114);
	LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLineCol(5, 4, "Value:");
	LCD_DrawCircle(160, 160, 30);
  LCD_SetTextColor(LCD_COLOR_GREEN);
	LCD_FillCircle(160, 160, 26);


	/*	restore last Font, Back Color*/
	LCD_RestoreFont();
	LCD_RestoreColors();
	
	delay_ms(2000);
	ReverseLCD();
	delay_ms(4000);
	NormalLCD();

	delay_ms(2000);
	LCD_DisplayOnTest();
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/


/**
  * @brief  Reads RGB Image (16 bpp). (ILI9328)
  * @param  Xpos:  X position in the LCD
  * @param  Ypos:  Y position in the LCD
  * @param  Xsize: X size in the LCD
  * @param  Ysize: Y size in the LCD
  * @param  pdata: Pointer to the RGB Image address.
  * @retval None
  */
// For (ILI9328) chip
void LCD_ReadRGBImage(uint16_t x0, uint16_t y0, uint16_t xSize, uint16_t ySize, uint8_t *pdata)
{
//  uint32_t index, size;
  uint16_t i, j;
	volatile uint16_t data1, data2;
  
	if (x0 > LCD_PIXEL_WIDTH) return;
	if (y0 > LCD_PIXEL_HEIGHT) return;
	if ((x0+xSize) > LCD_PIXEL_WIDTH) xSize = LCD_PIXEL_WIDTH+1 - x0;
	if ((y0+ySize) > LCD_PIXEL_HEIGHT) ySize = LCD_PIXEL_HEIGHT+1 - y0;
	
	x0 = LCD_PIXEL_WIDTH - x0;
#if (LANDSCAPE == 0)		// Portrait
	y0 = LCD_PIXEL_HEIGHT - y0;
#endif
  for(j = 0; j < ySize; j++)	// y-axis
	{
#if (LANDSCAPE == 1)
		LCD_WriteReg(0x20);				// set cursor X (actually y) 
		LCD_WriteData(y0+j);			// increasing
#else
		LCD_WriteReg(0x21);				// set cursor Y (actually y) 
		LCD_WriteData(y0-j);			// decreasing
#endif

		for(i = 0; i < xSize; i++)
		{
#if (LANDSCAPE == 1)
			LCD_WriteReg(0x21);				// set cursor Y (actually x) 
			LCD_WriteData(x0-i);			// decreasing
#else
		LCD_WriteReg(0x20);				// set cursor X (actually x) 
		LCD_WriteData(x0-i);			// decreasing
#endif
			LCD_WriteReg(0x22);	// 0x22: Memory Read	(ILI9328 CODE) same as Memory Write
			LCD_ReadData;   // the 1st return value is "dummy"
			data1 = LCD_ReadData;					
			//==>>> BGR --> RGB
			data2 = (uint16_t ) ((data1 & 0x003F)<<11) | ((data1 & 0xF800)>>11);
			data1 = data2 | (data1 & 0x07E0);
			//<<<===
			*((volatile uint16_t *) pdata)= data1;
			pdata += 2;
		}
	}
}
	
//?????????????????
//################## Low LEVEL DRIVER
/**
  * @brief  Reads the selected LCD Register.
  * @param  LCD_Reg: address of the selected register.
  * @retval LCD Register Value.
  */


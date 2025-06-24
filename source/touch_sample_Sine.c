#include "touch_module.h"
#include <stdio.h>	// for sprintf
#include <math.h>		// for sin
#include<stdbool.h>

#define ABS(X)  ((X) > 0 ? (X) : -(X))
#ifndef Bit
#define Bit(x) 	(0x01ul<<x)
#endif

extern volatile uint32_t msTick;

/* Private variables ---------------------------------------------------------*/
static TS_StateTypeDef  TS_State;

/* Private function prototypes -----------------------------------------------*/
void delay_ms(uint32_t wait_ms);
static void Draw_sample(void);
void save_backImage(uint16_t cx, uint16_t cy);
void reDraw_backImage(uint16_t cx, uint16_t cy);
void LCD_FillBall(uint16_t Xc, uint16_t Yc, uint16_t Rad);
static bool ball_moving = true;

#define BackColor	YELLOW	

/**
  * @brief  Touchscreen
  * @param  None
  * @retval None
  */
void Touch_sample_Sine (void)
{ 
  uint16_t x, y;
	uint32_t last_ms;
	uint16_t c_x;
	double c_y;

#define KEY_LONGTIME 10 
	uint16_t KeyTimer = KEY_LONGTIME;  

 	//----------------------------------
	LCD_SaveColors();
	LCD_SaveFont();
	
	LCD_Clear(BackColor);
	while(GPIOA->IDR & 0x01)			// wait until release KEY1
    delay_ms(20);						// wait 20 msec for debouncing
	WaitForTouchRelease(3);

//===========>>>>
			LCD_SetFont(&Font20);
			LCD_SetColors(RED, BLUE);
//>>>--------------------------------
			x = BSP_LCD_GetXSize();
			y = BSP_LCD_GetYSize();
			LCD_DisplayStringAt(x/2-70, y-60, " Sample Sine", LEFT_MODE);
//<<<--------------------------------
//-------------------------------
    while (TSC_TouchDet(1) == 0)
		{
			if(GPIOA->IDR & 0x01)						// normally low
			{
				if (--KeyTimer==0){
					LCD_RestoreColors();
					LCD_RestoreFont();
					return;
				}
			} else KeyTimer = KEY_LONGTIME;
		}
//-------------------------
			
			Touch_sensing(5, x/2-70, y-60, (Font20.Width)*12, (Font20.Height) );
			LCD_SetTextColor(BackColor);
			LCD_FillRect(x/2-70, y-60, (Font20.Width)*12, (Font20.Height));
			WaitForTouchRelease(5);
//<<<=================	
	//----------------------------------
 	Draw_sample();
	c_x = 0;
	c_y = 120;
	save_backImage(c_x, c_y);

	LCD_SetTextColor(MAGENTA);
//	LCD_FillCircle(c_x, c_y, 10);
	LCD_FillBall(c_x, c_y, 10);
	last_ms = msTick;

	while (1)
  {
		TS_GetState(&TS_State);
     x = TS_State.x;
     y = TS_State.y;
		
//>>>----------Assignment-----
		if(TS_State.TouchDetected)
		{
				if((y >= (c_y - 10))&& (y <= (c_y + 10)))
				{
					
					if((x >= (c_x - 10))&& (x <= (c_x + 10)))
					{
						last_ms = msTick;
					}

				}
		}	//	END of if(TS_State.TouchDetected)

//<<<-------------------------

		if ((msTick - last_ms) > 500 )			// every 500 ms 
		{
			last_ms = msTick;
			reDraw_backImage(c_x, (int) c_y);
			
			if(c_x++  == 320) c_x = 0;
			c_y = 120+100*sin(3.1415926535*c_x/36);
			save_backImage(c_x, (int) c_y);

			LCD_SetTextColor(MAGENTA);
//			LCD_FillCircle(c_x, (int) c_y, 10);
			LCD_FillBall(c_x, c_y, 10);
		}
		
    if(GPIOA->IDR & Bit(0))						// normally low
    {
			if (--KeyTimer==0){
				LCD_RestoreColors();
				LCD_RestoreFont();
				return;
			}
		} else KeyTimer = KEY_LONGTIME;
    
    
    delay_ms(10);
  }
}
//=========================================================

static void Draw_sample(void)
{
	uint16_t x;
	double y0, y1;

	LCD_Clear(BackColor);

	LCD_SetColors(BLUE, BackColor);
	LCD_DisplayStringLineCol(0, 0, " Touch ball to stop it!");
	LCD_DrawHLine(0, 120, 320);

	LCD_SetTextColor(RED);
		for(x=0; x<320; x++)
		{
			y0 = 120+100*sin(3.1415926535*x/36);
			y1 = 120+100*sin(3.1415926535*(x+1)/36);
			LCD_DrawLine(x, (int)y0, x+1, (int)y1);
		}
}

#define Image_width  21
#define Image_height 21
#define half_w	(Image_width/2)
#define half_h	(Image_height/2)
uint8_t pImage[(Image_width*Image_height*2)];

void save_backImage(uint16_t cx, uint16_t cy)
{
	if (cx < half_w) cx = 0;
	else cx = cx -half_w;
	if (cy < half_h) cy = 0;
	else cy = cy -half_h;
	
	LCD_ReadRGBImage(cx, cy, Image_width, Image_height, pImage);	
}

void reDraw_backImage(uint16_t cx, uint16_t cy)
{
	if (cx < half_w) cx = 0;
	else cx = cx -half_w;
	if (cy < half_h) cy = 0;
	else cy = cy -half_h;

	LCD_DrawRGBImage(cx, cy, Image_width, Image_height, pImage);
}

void LCD_FillBall(uint16_t Xc, uint16_t Yc, uint16_t Rad)
{
uint16_t rs, sh, Tcolor, Bcolor;
	
	LCD_FillCircle(Xc, Yc, Rad);
	LCD_GetColors(&Tcolor, &Bcolor);
	LCD_SetTextColor(WHITE);
	rs = Rad * 0.5;
	sh = Rad * 0.2;
	LCD_FillCircle(Xc-sh, Yc-sh, rs);
	LCD_SetTextColor(Tcolor);
	LCD_FillCircle(Xc, Yc, rs);
	LCD_SetTextColor(WHITE);
	rs = Rad * 0.1;
	if (rs == 0) rs = 1;
	sh = (Rad * 0.7)*0.7;			// cos(45deg)*0.7
	LCD_FillCircle(Xc+sh, Yc+sh, rs);
	LCD_SetTextColor(Tcolor);
}

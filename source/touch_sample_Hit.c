#include "touch_module.h"
#include <stdio.h>	// for sprintf
#include "RNG.h"

#ifndef Bit
#define Bit(x) 	(0x01ul<<x)
#endif

#define point_Count  5
Point Points[point_Count]={ 
{5, 5}, {315,5}, {5, 235}, {315, 235}, {160, 120} }; 

/* Private variables ---------------------------------------------------------*/
static TS_StateTypeDef  TS_State;
/* Private function prototypes -----------------------------------------------*/
void delay_ms(uint32_t wait_ms);
static void Draw_sample(Point* pPts, uint16_t cnt);
void Draw_cross(uint16_t x, uint16_t y, uint16_t Color, uint16_t CenColor);

#define Num_praises 4
/**
  * @brief  Touchscreen play & pause
  * @param  None
  * @retval None
  */
void Touch_sample_Hit (void)
{ 
	char praises[Num_praises][10] = {" Bravo ", " Superb ", "Way to go", "My Idol"};
  uint16_t x, y, i;
  uint8_t state[point_Count], C_finish, prIndex=0;

#define KEY_LONGTIME 10 
#define BackColor	LCD_COLOR_GREEN	
	uint16_t KeyTimer = KEY_LONGTIME;  
	uint8_t RNG_setup = 0;
	
	if (RNG_setup == 0){
		RNG_setup = RNG_Init();	// 1 = RNG OK
	}
 	//----------------------------------
re_start:	
	LCD_SaveColors();
	LCD_SaveFont();
	LCD_SetFont(&Font16);
	
	LCD_Clear(BackColor);
	while(GPIOA->IDR & 0x01)			// wait until release KEY1
    delay_ms(20);						// wait 20 msec for debouncing
	WaitForTouchRelease(3);
	
	//----------------------------------
 	Draw_sample(Points, point_Count);
	LCD_SetColors(LCD_COLOR_DARKBLUE, BackColor);
	LCD_DisplayStringLineCol(3, 2, "Touch Centers of Crosses!");					
	C_finish = point_Count;
	for (i=0; i<point_Count; i++)
	{
		state[i] = 0;			// state of cross point i:  0= to be hit; 1= vanished
	}

	while (1)
  {
      TS_GetState(&TS_State);
      x = TS_State.x;
      y = TS_State.y;

		if(TS_State.TouchDetected)
		{
				char text[64];
//>>>------Assignment-----------------------
		//###==== 	
		 for (i=0; i<point_Count; i++)
		 {
			if( state[i] == 0 ) // if cross point i was not yet hit
			{						// judge if touching point in the square of 7x7 pixels, i.e., -+3
        if((y >= (Points[i].Y - 3))&& (y <= (Points[i].Y + 3)))
        {
            if((x >= (Points[i].X - 3))&& (x <= Points[i].X + 3))
            {
//================
					LCD_SetColors(LCD_COLOR_RED, BackColor); // Text = red; 
						sprintf(text, "Hit Point: x=%04d y=%04d", x, y);
					LCD_DisplayStringAt((Font20.Width)*1.7, (Font20.Height)*10.5, text, LEFT_MODE);
//================	

						state[i] = 1;		// state of cross point i has been hit
						C_finish --;		// decrement
						x = Points[i].X;					// used in function "Draw_cross"
						y = Points[i].Y;					// used in function "Draw_cross"
						Draw_cross(x,y,BackColor,BackColor);		// call "Draw_cross": delete by drawing with BackColor
						break;
					}	// END of if (range of x)
				}	  // END of if (range of y)
			}		// END of "if( state[i] == 0 )"
		 }		// END of "for (i=0; i<point_Count; i++)"
//<<<<--------------------------
		 
			if (C_finish == 0){
				LCD_Clear(BackColor);
				LCD_SetFont(&Font20);
				LCD_SetColors(LCD_COLOR_RED, LCD_COLOR_BLUE);
				x = BSP_LCD_GetXSize();
				y = BSP_LCD_GetYSize();

				if (RNG_setup == 1){
					prIndex = RNG_Get_RandomRange(0, Num_praises-1);
					LCD_DisplayStringAt(x/2-60, y-60, praises[prIndex], LEFT_MODE);

						LCD_SetFont(&Font16);
						LCD_SetColors(BLUE, BackColor); // Text = red; 
						sprintf(text, "prIndex = %01d", prIndex);
						LCD_DisplayStringLineCol(9, 4, text);					
						LCD_SetColors(LCD_COLOR_RED, BackColor); // Text = red; 
				} else{
					LCD_DisplayStringAt(x/2-60, y-60, praises[prIndex++], LEFT_MODE);
					if(prIndex == Num_praises) prIndex = 0;
				}

				Touch_sensing(5, x/2-60, y-60, (Font20.Width)*9, (Font20.Height) );
				LCD_SetTextColor(BackColor);
				LCD_FillRect(x/2-60, y-60, (Font20.Width)*9, (Font20.Height));
				WaitForTouchRelease(5);
				goto re_start;
			}
		
		}	//	END of if(TS_State.TouchDetected)

	
    if(GPIOA->IDR & Bit(0))						// normally low
    {
			RNG_Close();

			if (--KeyTimer==0){
				LCD_RestoreColors();
				LCD_RestoreFont();
				return;
			}
		} else KeyTimer = KEY_LONGTIME;
    
    
    delay_ms(10);
  }
}

//--------------------------------
void Vline_draw(int16_t x, int16_t y, uint16_t lg)
{
	if (x < 0) return;
	if (y < 0){
		lg = lg + y;
		y = 0;
	}
	LCD_DrawVLine(x, y, lg);
}

//-----------------------------
void Hline_draw(int16_t x, int16_t y, uint16_t lg)
{
	if (y < 0) return;
	if (x < 0){
		lg = lg + x;
		x = 0;
	}
	LCD_DrawHLine(x, y, lg);
}


//=========================================================
void Draw_cross(uint16_t x, uint16_t y, uint16_t textColor, uint16_t CenColor)
{
	#define L	 16
	#define S		L-2
	#define hL	L/2
	LCD_SetTextColor(textColor);
	
	Vline_draw(x-1, y-(hL-1), S);
	Vline_draw(x, y-hL, L);
	Vline_draw(x+1, y-(hL-1), S);
	Hline_draw(x-(hL-1), y-1, S);
	Hline_draw(x-hL, y, L);
	Hline_draw(x-(hL-1), y+1, S);
	LCD_DrawPixel(x, y, CenColor);
}

static void Draw_sample(Point* pPts, uint16_t cnt)
{
	uint16_t xp, yp,  i;
	

	for (i=0; i<cnt; i++)
	{
			xp = pPts[i].X;
			yp = pPts[i].Y;
			Draw_cross( xp, yp, LCD_COLOR_RED, LCD_COLOR_WHITE);
	}
}



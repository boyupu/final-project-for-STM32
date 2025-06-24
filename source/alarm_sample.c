#include "touch_module.h"
#include <stdio.h>	// for sprintf

#ifndef Bit
#define Bit(x) 	(0x01ul<<x)
#endif

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void delay_ms(uint32_t wait_ms);
void Touch_TextBox_Draw(uint16_t textColor, uint16_t backColor, uint8_t Line, 
	uint16_t Col, uint8_t BlankLines, char *ptr);
uint8_t Check_TextBox_touch(uint8_t Line, uint16_t Col, uint8_t NrChar, uint8_t BlankLines);

void BoxTriangle(uint8_t UP_DW, uint16_t x, uint16_t y, uint16_t Wd, uint16_t Ht);
void Draw_SUN(uint8_t x_c, uint16_t y_c, uint16_t r);
void Draw_Time(uint8_t time, uint8_t line, uint16_t col);
void BCD_Plus_1(uint8_t *time);
void BCD_Minus_1(uint8_t *time);

/**
  * @brief  Touchscreen play & pause
  * @param  None
  * @retval None
  */
void Sample_alarmA (void)
{ 
uint16_t i, j;
uint8_t W_day=0, hour, min, sec;
uint32_t tmpreg;

	
#define BackColor	GREEN	
#define TextColor	DARKBLUE
#define x1_U 52
#define x2_U (x1_U+85)
#define x3_U (x2_U+85)
#define y1_U 40
#define y1_D 96
#define	boxWd	32
#define boxHt	28	
#define UP 1
#define DOWN 0	
#define f24Wd 17	
#define f24Ht 24
#define WdayHt 168
#define TimeLine 3
	//----------------------------------
	LCD_SaveColors();
	LCD_SaveFont();
	
	LCD_Clear(BackColor);
	
	//----------------------------------
	LCD_SetColors(RED, BackColor);
	LCD_SetFont(&Font20);
	LCD_DisplayStringLineCol(0, 4, "Alarm-A Setting");					
	LCD_SetFont(&Font24);
	BoxTriangle(UP, x1_U, y1_U, boxWd, boxHt);
	BoxTriangle(UP, x2_U, y1_U, boxWd, boxHt);
	BoxTriangle(UP, x3_U, y1_U, boxWd, boxHt);
	BoxTriangle(DOWN, x1_U, y1_D, boxWd, boxHt);
	BoxTriangle(DOWN, x2_U, y1_D, boxWd, boxHt);
	BoxTriangle(DOWN, x3_U, y1_D, boxWd, boxHt);
	LCD_SetTextColor(TextColor);

	W_day = (uint8_t) (RTC->DR >> 13) & 0x07; 
	for (i=0, j=f24Wd*3; i<7; i++)
	{
		
		if (W_day == i)	LCD_FillRect(j, WdayHt, f24Wd, f24Wd);			
		else LCD_DrawRect(j, WdayHt, f24Wd, f24Wd);			
		j += (2*f24Wd);
	}
	LCD_DisplayStringLineCol(6, 3, "  M T W T F S");					
	Draw_SUN(f24Wd*3.5, f24Ht*6.4, f24Wd*0.5);
  W_day |= Bit(4);

	LCD_DisplayStringLineCol(TimeLine, 3, "00 : 00 : 00");					

		tmpreg = RTC->TR; 
		hour	 = ((uint8_t) (tmpreg>>16)) & 0x3F;
		min	 = ((uint8_t) (tmpreg>>8));
		sec	 = ((uint8_t) tmpreg);
	Draw_Time(hour, TimeLine, 3);
	Draw_Time(min, TimeLine, 3+5);
	Draw_Time(sec, TimeLine, 3+10);

	LCD_SetFont(&Font20);

	Touch_TextBox_Draw(WHITE, MAGENTA, 10, 5, 0, "  OK  ");
	Touch_TextBox_Draw(WHITE, MAGENTA, 10, 12, 0, "Cancel");

	WaitForTouchRelease(1);

#define updatePeriod 40
	while (1)
  {
		uint16_t x, y;
		TS_StateTypeDef  TS_State;

loop_start:
		delay_ms(10);
		if (i > 0) i--;

		TS_GetState(&TS_State);
		if(!TS_State.TouchDetected)	i=0;			// counting end if no touched
		else		// if touched
		{
			//<<<<---------------------------------------------------
			if (Check_TextBox_touch(10, 5, 6, 0))	// OK box
			{
					Touch_TextBox_Draw(GRAY, DARKRED, 10, 5, 0, "  OK  ");
//					Set_Alarm_A(hour, min, sec, W_day);
					goto release;
			} else{
				if (Check_TextBox_touch(10, 12, 6, 0))	// Cancel box
				{
					Touch_TextBox_Draw(GRAY, DARKRED, 10, 12, 0, "Cancel");
					// disable Alarm A and its interrupt
//					RTC->CR &= ~(RTC_CR_ALRAE | RTC_CR_ALRAIE);	// CLEAR bits 8 and 12
					goto release;
				}
			}
			//<<<----------------------------------------------------

			if ( i == 0)		// no touch period >= 0.1s
			{
				i= updatePeriod;
				x = TS_State.x;
				y = TS_State.y;
				//>>>-------- Up Boxes -----------------------
				if((y >= y1_U) && (y <= (y1_U + boxHt)) )
				{
					if((x >= x1_U) && (x <= (x1_U + boxWd)) )
					{
						if(hour == 0x23) hour = 0x00;
						else	BCD_Plus_1(&hour);
						Draw_Time(hour, TimeLine, 3);
					} else {
					if((x >= x2_U) && (x <= (x2_U + boxWd)) )
					{
						if(min == 0x59) min = 0x00;
						else	BCD_Plus_1(&min);
						Draw_Time(min, TimeLine, 3+5);
					} else{
						if((x >= x3_U) && (x <= (x3_U + boxWd)) )
						{
							if(sec == 0x59) sec = 0x00;
							else	BCD_Plus_1(&sec);
							Draw_Time(sec, TimeLine, 3+10);
						}
						}
					}
					goto loop_start;
				}				
				//<<<---------------------------------------------
				//>>>-------- Down Boxes -----------------------
				if((y >= y1_D) && (y <= (y1_D + boxHt)) )
				{
					if((x >= x1_U) && (x <= (x1_U + boxWd)) )
					{
						if(hour == 0) hour = 0x23;
						else	BCD_Minus_1(&hour);
						Draw_Time(hour, TimeLine, 3);
					} else {
						if((x >= x2_U) && (x <= (x2_U + boxWd)) )
						{
							if(min == 0x00) min = 0x59;
							else	BCD_Minus_1(&min);
							Draw_Time(min, TimeLine, 3+5);
						} else{
						if((x >= x3_U) && (x <= (x3_U + boxWd)) )
						{
							if(sec == 0x00) sec = 0x59;
							else	BCD_Minus_1(&sec);
							Draw_Time(sec, TimeLine, 3+10);
						}
						}
					}
					goto loop_start;
				}
			}  // END of if (i==0)
				
				//<<<---------------------------------------------
				//>>>+++++++++++++++ Weekday Boxes ++++++++++++++++++++
				if((y >= WdayHt) && (y <= (WdayHt+ f24Wd)) )
				{
					uint8_t ii, jj;
					j = f24Wd*3;
					for(ii=0; ii<7; ii++)
					{
						if((x >= j) && (x <= (j+f24Wd)) )
						{			
							LCD_SetTextColor(BackColor);
							// W_day: 16=Sunday; 16+1: Monday,....
							if (W_day == (0x10+ii)){
									LCD_FillRect(j+1, WdayHt+1, f24Wd-2, f24Wd-2);		// no weekday selection	
									W_day = 0;			// de-select weekday
									LCD_SetTextColor(TextColor);
							} else {
									if (W_day>=0x10){
										jj = f24Wd*(3+(W_day-16)*2);
										LCD_FillRect(jj+1, WdayHt+1, f24Wd-2, f24Wd-2);	// clear old weekday
									}
									LCD_SetTextColor(TextColor);
									LCD_FillRect(j, WdayHt, f24Wd, f24Wd);			// set new weekday
									W_day = 0x10+ii;
							}
							WaitForTouchRelease(2);
							break;
						}
						j += f24Wd*2;
					}
				}				
				//<<<+++++++++++++++++++++++++++++++++++++++++++++++++
		}	//	END of if(TS_State.TouchDetected)
	
  }				// end of while(1) LOOP

release:
		WaitForTouchRelease(3);
		LCD_RestoreColors();
		LCD_RestoreFont();
}

//------------
//---------------------
void BoxTriangle(uint8_t UP_DW, uint16_t x, uint16_t y, uint16_t Wd, uint16_t Ht)
{
uint16_t	X_tip, Y_tip, R_c;
int16_t	y_H;
Point Points_UP[3];
	
	X_tip = x+Wd/2;
	R_c = Wd - 4;
	if (UP_DW){				// if up-triangle
		Y_tip = y+3;
		y_H = (R_c*0.8);
	} else {					// if down-triangle
		Y_tip = y+Ht-3;
		y_H = -(R_c*0.8);
	}
	Points_UP[0].X = X_tip;
	Points_UP[0].Y = Y_tip;
	Points_UP[1].X = X_tip-R_c/2;
	Points_UP[1].Y = Y_tip+y_H;
	Points_UP[2].X = X_tip+R_c/2;
	Points_UP[2].Y = Y_tip+y_H;

	LCD_SetTextColor(WHITE);
  LCD_FillRect(x, y, Wd, Ht);			
	LCD_SetTextColor(TextColor);
  LCD_DrawRect(x, y, Wd, Ht);			
	LCD_FillTriangle(Points_UP);
}

//-----------------------------
void Draw_SUN(uint8_t x_c, uint16_t y_c, uint16_t r)
{
	uint16_t Tcolor, Bcolor, rm=r-2;
	
	
	LCD_GetColors(&Tcolor, &Bcolor);
	LCD_SetTextColor(YELLOW);
	LCD_FillCircle(x_c, y_c, r*0.6);
	LCD_DrawHLine(x_c-r, y_c, r*2);
	LCD_DrawVLine(x_c, y_c-r, r*2);
	LCD_DrawLine(x_c-rm, y_c-rm, x_c+rm, y_c+rm);
	LCD_DrawLine(x_c+rm, y_c-rm, x_c-rm, y_c+rm);
	LCD_SetTextColor(Bcolor);
	LCD_DrawCircle(x_c, y_c, r*0.6+1);
	LCD_DrawCircle(x_c, y_c, r*0.6+2);
	LCD_SetTextColor(Tcolor);
}

//-----------------------------
void BCD_Plus_1(uint8_t *time)
{
uint8_t temp;
		temp = *time;
		if ((temp & 0x0F) == 0x09) temp +=0x07;
		else temp ++;
		*time = temp;
}	

//-----------------------------
void BCD_Minus_1(uint8_t *time)
{
uint8_t temp;
		temp = *time;
		if ((temp & 0x0F) == 0x00) temp -=0x07;
		else temp --;
		*time = temp;
}	

//-----------------------------
void Draw_Time(uint8_t time, uint8_t line, uint16_t col)
{
	char timeASCII[3];
	sFONT *pOld_font;
	sprintf((char*) timeASCII,"%02x", time);
	pOld_font = LCD_GetFont();
	LCD_SetFont(&Font24);
	LCD_DisplayStringLineCol(line, col,  timeASCII);
	LCD_SetFont(pOld_font);
}


#include "touch_module.h"
#include <stdio.h>	// for sprintf

#define ABS(X)  ((X) > 0 ? (X) : -(X))
#ifndef Bit
#define Bit(x) 	(0x01ul<<x)
#endif

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void delay_ms(uint32_t wait_ms);
static void Draw_sample(void);

#define R_w	44
#define	R_h	60
#define ColorBoxes_x	(319-R_w)
#define Color0_y	0
#define Color1_y	(R_h-1)
#define Color2_y	(2*R_h-1)
#define Color3_y	(3*R_h-1)
void Touch_ChooseColor(uint16_t Val, uint16_t lastVal);

/**
  * @brief  Touchscreen
  * @param  None
  * @retval None
  */
void Touch_sample_FreeDraw (void)
{ 
	TS_StateTypeDef  TS_State;
  uint16_t x, y;
	uint16_t TouchColor;
#define KEY_LONGTIME 10 
#define BackColor	WHITE	
	uint32_t KeyTimer = KEY_LONGTIME;  

 	//----------------------------------
//re_start:	
	
	LCD_SaveColors();
	LCD_SaveFont();
	
	LCD_Clear(BackColor);
	while(GPIOA->IDR & 0x01)			// wait until release KEY1
    delay_ms(20);						// wait 20 msec for debouncing
	WaitForTouchRelease(3);

//===========>>>>
			LCD_SetFont(&Font20);
			LCD_SetColors(RED, BLUE);
			x = BSP_LCD_GetXSize();
			y = BSP_LCD_GetYSize();
			LCD_DisplayStringAt(x/2-60, y-60, (char*)"  PLAY  ", LEFT_MODE);
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
			Touch_sensing(5, x/2-60, y-60, (Font20.Width)*9, (Font20.Height) );
			LCD_SetTextColor(BackColor);
			LCD_FillRect(x/2-60, y-60, (Font20.Width)*9, (Font20.Height));
			WaitForTouchRelease(5);
//<<<=================	
	//----------------------------------
 	Draw_sample();

	while (1)
  {
	uint16_t x0, y0;

		TS_GetState(&TS_State);
     x = TS_State.x;
     y = TS_State.y;
//		if(x!=x0 || y!=y0){				// TEST 1
			if((ABS(x-x0)+ABS(y-y0)) > 1){	//TEST 2 is good
//			if((ABS(x-x0)+ABS(y-y0)) > 2){	// TEST 3
				TS_State.TouchDetected = 0;
				x0 = x;
				y0 = y;
		}
//>>>----------Assignment-----
		if(TS_State.TouchDetected)
		{
				if(x < ColorBoxes_x){		// in range of drawing plate
					LCD_SetTextColor(TouchColor);	
					LCD_FillRect(x, y, 2, 2);		// draw a square point at (x,y)
				} else{  
					  if(y >= Color0_y && (y < Color1_y)){ // i.e., ColorBoxes_x <= x <= 319
							LCD_SetTextColor(BackColor);									// change the text color
							LCD_FillRect(0, 0, ColorBoxes_x,BSP_LCD_GetYSize()); // erase the drawing plate
						}else if(y >= Color1_y && (y < Color2_y)){
								if(TouchColor != BLUE){
									Touch_ChooseColor(BLUE, TouchColor );
									TouchColor = BLUE;
								}
						}else if((y >= Color2_y) && (y < Color3_y)){
							  if(TouchColor != GREEN){
									Touch_ChooseColor(GREEN, TouchColor );
									TouchColor = GREEN;
							  }
						}else{
							if((y >= Color3_y) && (y < (4*R_h-1))){
								if(TouchColor != RED){
							    Touch_ChooseColor(RED, TouchColor );
									TouchColor = RED;
							  }
						  } 
				}
			}			
		}
//<<<-------------------------
		
    if(GPIOA->IDR & Bit(0))						// normally low
    {
			if (--KeyTimer==0){
				LCD_RestoreColors();
				LCD_RestoreFont();
				return;
			}
		} else KeyTimer = KEY_LONGTIME;
    
//    delay_ms(1);
  }
}



//=========================================================
void Touch_ChooseColor(uint16_t Val, uint16_t lastVal)
{
		LCD_SetTextColor(BLACK);
		switch(Val)
		{
				case BLUE:  //TouchColor = BLUE;  draw BLACK boundary  
										LCD_DrawRect(ColorBoxes_x, Color1_y, R_w, R_h);
										LCD_DrawRect(ColorBoxes_x+1, Color1_y, R_w-2, R_h-2);
										break;
			  case GREEN: //TouchColor = GREEN; draw BLACK boundary 
										LCD_DrawRect(ColorBoxes_x, Color2_y, R_w, R_h);
										LCD_DrawRect(ColorBoxes_x+1, Color2_y+1, R_w-2, R_h-2);
										break;
				case RED:  	//TouchColor = RED;  draw BLACK boundary
										LCD_DrawRect(ColorBoxes_x, Color3_y, R_w, R_h);
										LCD_DrawRect(ColorBoxes_x+1, Color3_y+1, R_w-2, R_h-2);
										break;
			  default:	break;
		}
		
		LCD_SetTextColor(lastVal);
		switch(lastVal)
		{
				case BLUE:  
										LCD_DrawRect(ColorBoxes_x, Color1_y, R_w, R_h);
										LCD_DrawRect(ColorBoxes_x+1, Color1_y+1, R_w-2, R_h-2);
										break;
			  case GREEN:   
										LCD_DrawRect(ColorBoxes_x, Color2_y, R_w, R_h);
										LCD_DrawRect(ColorBoxes_x+1, Color2_y+1, R_w-2, R_h-2);
										break;
				case RED:  	
										LCD_DrawRect(ColorBoxes_x, Color3_y, R_w, R_h);
										LCD_DrawRect(ColorBoxes_x+1, Color3_y+1, R_w-2, R_h-2);
										break;
			  default:	break;
		}
}

static void Draw_sample(void)
{
	LCD_Clear(WHITE);
  LCD_SetTextColor(GRAY);
	LCD_FillRect(ColorBoxes_x, Color0_y, R_w, R_h);
  LCD_SetTextColor(BLUE);
	LCD_FillRect(ColorBoxes_x, Color1_y, R_w, R_h);
  LCD_SetTextColor(GREEN);
	LCD_FillRect(ColorBoxes_x, Color2_y, R_w, R_h);
	LCD_SetTextColor(RED);
	LCD_FillRect(ColorBoxes_x, Color3_y, R_w, R_h);
	LCD_SetColors(MAGENTA, GRAY);
	LCD_DisplayStringAt(ColorBoxes_x, (R_h/3 +4), (char*)"CLR", LEFT_MODE);
		// initial pen color
	uint16_t TouchColor = RED; 
		Touch_ChooseColor(TouchColor, WHITE);
}



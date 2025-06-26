#include "stm32f4xx.h"
#include "stm324xg_lcd_sklin.h"
#include "user_defined.h"
#include "touch_module.h"
#include "game.h"
#include "baseball.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h> 

//constants

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define LCD_W   BSP_LCD_GetXSize()
#define LCD_H   BSP_LCD_GetYSize()
//===================================
int legendUnlocked = 0;

//=====================================
//Background color

static inline uint16_t GetBGColor(int x, int y)
{
    if (y <  30)  return DARKBLUE;
    if (y <  45)  return GRAY;
    if (y < 160)  return DARKBLUE;

    int dx = x - 160;
    int dy = y - 175;
    if (dx * dx + dy * dy <= 20 * 20)
        return DARKGREEN;
    return GREEN;
}

//======================================

void ClearBallRegion(int cx, int cy, int radius)
{
    int R2 = (radius) * (radius); 

    for (int dy = -radius; dy <= radius; ++dy) {
        int py = cy + dy;
        if (py < 0 || py >= BSP_LCD_GetYSize()) continue;

        for (int dx = -radius; dx <= radius; ++dx) {
            int px = cx + dx;
            if (px < 0 || px >= BSP_LCD_GetXSize()) continue;

            if (dx * dx + dy * dy <= R2) {
                LCD_SetTextColor(GetBGColor(px, py));
                LCD_DrawPixel(px, py, GetBGColor(px, py));
            }
        }
    }
}
//========================================

static void DrawBallMasked(int cx, int cy, const uint16_t *img)
{
    const int r = 16;               /* bitmap is 32×32 */
    const int w = 32;
    for (int dy = -r; dy <= r; ++dy) {
        for (int dx = -r; dx <= r; ++dx) {
            if (dx * dx + dy * dy > r * r) continue;      /* skip outside circle */
            int px = cx + dx, py = cy + dy;
            if (px < 0 || px >= LCD_W || py < 0 || py >= LCD_H) continue;
            uint16_t c = img[(dy + r) * w + (dx + r)];     /* from bitmap */
            if (c == 0xffff) continue;                     /* skip key colour */
            LCD_DrawPixel(px, py, c);
        }
    }
}

//==========================================

void DrawAudience(void)
{
    int screenW = BSP_LCD_GetXSize();
    int bandHeight = 12;

    for (int y = 0; y < 30; ++y) {
        for (int x = 0; x < screenW; ++x) {
            uint16_t c;
            switch ((x + y) % 4) {
                case 0: c = LIGHTBLUE; break;
                case 1: c = WHITE;     break;
                case 2: c = GRAY;      break;
                default: c = DARKGRAY; break;
            }
            LCD_DrawPixel(x, y, c);
        }
    }
}
static void RedrawFixedStartElements(void)
{
    LCD_SetTextColor(WHITE);
    LCD_SetBackColor(DARKBLUE);
    LCD_SetFont(&Font24);
    LCD_DisplayStringLineCol(4,2, " BASEBALL GAME ");

    LCD_SetTextColor(GREEN);
    LCD_FillRect(0, 160, LCD_W, 80);
    LCD_SetTextColor(DARKGREEN);
    LCD_FillCircle(160, 175, 20);
    LCD_SetTextColor(WHITE);
    LCD_FillCircle(160, 240, 10);
	  DrawAudience();

    const uint16_t btnX = 80, btnY = 175, btnW = 160, btnH = 40;
    LCD_SetTextColor(BLUE);
    LCD_FillRect(btnX, btnY, btnW, btnH);
    LCD_SetTextColor(WHITE);
    LCD_SetBackColor(BLUE);
    LCD_SetFont(&Font20);
    LCD_DisplayStringAt(btnX + 40, btnY + 12, (uint8_t *)"START", LEFT_MODE);
}
//===============================
void DrawLegendBatter(void)
{
    int x0 = 200;
    int y0 = 150;

    LCD_SetTextColor(BLUE);
    for (int y = 0; y < 10; y++) {
        for (int x = 10 - y; x < 40 + y; x++) {
            LCD_DrawPixel(x0 + x, y0 + y, BLUE);
        }
    }

    LCD_SetTextColor(YELLOW);
    for (int y = 10; y < 20; y++) {
        for (int x = 20; x < 40; x++) {
            LCD_DrawPixel(x0 + x, y0 + y, YELLOW);
        }
    }

    LCD_SetTextColor(WHITE);
    for (int y = 20; y < 45; y++) {
        for (int x = 15; x < 45; x++) {
            LCD_DrawPixel(x0 + x, y0 + y, WHITE);
        }
    }

    LCD_SetTextColor(YELLOW);
    for (int y = 25; y < 35; y++) {
        for (int x = 5; x < 15; x++) {
            LCD_DrawPixel(x0 + x, y0 + y, YELLOW);
        }
    }

    LCD_SetTextColor(BROWN);
    for (int i = 0; i < 30; i++) {
        int bx = x0 + 45 + i;
        int by = y0 + 10 - i / 2;
        if (bx < 320 && by >= 0) {
            LCD_DrawPixel(bx, by, BROWN);
        }
    }

    LCD_SetTextColor(BLACK);
    for (int y = 45; y < 65; y++) {
        for (int x = 18; x < 24; x++) {
            LCD_DrawPixel(x0 + x, y0 + y, BLACK);
        }
        for (int x = 32; x < 38; x++) {
            LCD_DrawPixel(x0 + x, y0 + y, BLACK);
        }
    }

    LCD_SetTextColor(RED);
    for (int x = 16; x <= 26; x++) {
        LCD_DrawPixel(x0 + x, y0 + 65, RED);
    }
    for (int x = 30; x <= 40; x++) {
        LCD_DrawPixel(x0 + x, y0 + 65, RED);
    }
}
//===============================
void ShowLegendEasterEgg(void)
{
    LCD_Clear(DARKYELLOW);
	
	  LCD_SetFont(&Font20);
	  LCD_SetBackColor(DARKYELLOW);
	  LCD_DisplayStringLineCol(6, 4, "Maker:");
    LCD_DisplayStringLineCol(8, 4, "BOYUPU");
	  LCD_DisplayStringLineCol(10, 4, "113511107");
	
    for (int i = 0; i < 10; ++i) {
        LCD_SetTextColor((i % 2 == 0) ? YELLOW : ORANGE);
        LCD_SetFont(&Font20);
        LCD_DisplayStringLineCol(3, 4, "LEGEND UNLOCKED!");
        delay_ms(400);
        LCD_SetTextColor(WHITE);
        LCD_DisplayStringLineCol(3, 4, "LEGEND UNLOCKED!");
        delay_ms(200);
    }
		DrawLegendBatter();

    delay_ms(2000);

    legendUnlocked = 1;
		LCD_SetBackColor(DARKBLUE);
}
//===============================
//Opening
static void ShowStartScreen(void)
{
     LCD_Clear(DARKBLUE);
     DrawAudience();
     RedrawFixedStartElements();

    typedef struct { int x, y; } Key;
    const Key path[] = {
        { -30, 240 }, {  10, 220 }, {  40, 195 }, {  70, 170 },
        { 100, 148 }, { 130, 135 }, { 165, 135 }, { 200, 140 },
        { 230, 155 }, { 255, 175 }, { 275, 185 }, { 290, 190 }
    };
    const int keyCnt = sizeof(path) / sizeof(path[0]);

    int px = -1, py = -1;

    for (int seg = 0; seg < keyCnt - 1; ++seg) {
        const int sub = 6;
        for (int s = 0; s <= sub; ++s) {
            float t = (float)s / sub;
            int x = path[seg].x + (int)((path[seg+1].x - path[seg].x) * t);
            int y = path[seg].y + (int)((path[seg+1].y - path[seg].y) * t);

            if (px >= 0) ClearBallRegion(px, py, 16);

					  LCD_SetTextColor(WHITE);
            LCD_FillCircle(x, y, 11);
            DrawBallMasked(x, y, baseballImg);

            px = x; py = y;
            delay_ms(25);
        }
    }

		int ballFinalX = 290, ballFinalY = 190, ballFinalR = 10;
    TS_StateTypeDef ts;
    int startPressed = 0;
    const uint16_t btnX = 80, btnY = 175, btnW = 160, btnH = 40;
		
    while (1) {
			
        TS_GetState(&ts);
        if (ts.TouchDetected &&
            ts.x >= btnX && ts.x <= btnX + btnW &&
            ts.y >= btnY && ts.y <= btnY + btnH) {
            startPressed = 1;
						delay_ms(120);
            while (ts.TouchDetected) TS_GetState(&ts);
            break;
        }
				int dx = ts.x - ballFinalX;
        int dy = ts.y - ballFinalY;
        if (dx * dx + dy * dy <= 100) {
                ShowLegendEasterEgg();
                LCD_Clear(BLACK);
                RedrawFixedStartElements();
                LCD_SetTextColor(WHITE);
                LCD_FillCircle(280, 190, 10);
        }
				
        delay_ms(25);
    }
    LCD_Clear(BLACK);
}
//====================
void InitRandomSeed(void) {
    TS_StateTypeDef ts;
    TS_GetState(&ts);
    srand(ts.x + ts.y + SysTick->VAL);
}
//====================
void stm32f4_Hardware_Init(void);
void Default_Calibration(void);
void Driver_GPIO(void);
//====================
int main(void)
{
	  srand(RTC->TR);
    stm32f4_Hardware_Init();
    Default_Calibration();
    LCD_Init();
    Driver_GPIO();
    LCD_DisplayOn();
	  InitRandomSeed();

    ShowStartScreen();
	  LCD_SetBackColor(BLACK);
    LCD_SetTextColor(WHITE);
	  LCD_SetFont(&Font16);

    while (1) {
        GameLoop();
        delay_ms(40);
    }
}


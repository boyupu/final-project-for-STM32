#include "game.h"
#include "stm324xg_lcd_sklin.h"
#include <stdlib.h>
#include <stdio.h>
#include "user_defined.h"
#include "touch_module.h"
#include <math.h>
#include "baseball.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
//===========================
int snakeFrame = 0;
#define SNAKE_AMPLITUDE 32
#define SNAKE_FREQ 0.25
int floatframe = 0;
#define FLOAT_AMP  5
#define FLOAT_FREQ 0.6f
//===========================

int opponentLevel   = 0;
float speedMulTbl[] = {1.05f, 1.1f, 1.15f, 1.2f};
uint8_t compRunTbl[] = {1, 3, 4, 5};

//===========================
#define BALL_RADIUS      6
#define STRIKEZONE_X     265
#define STRIKEZONE_Y     105
#define STRIKEZONE_W     40
#define STRIKEZONE_H     40
#define HIT_MARGIN       10
#define SWING_DURATION   5
#define BAT_RADIUS       7
//===========================
int practiceMode = 1;
int hitCount = 0;
int pkButtonDrawn = 0;
int playerOut = 0;
int computerScore = 0;
int showSwitchMessage = 0;
int waitAfterSwitch = 0;
//============================
Ball currentBall;
int swingFlag = 0;
int swingTimer = 0;
int ballInZone = 0;
int ballAlreadyHit = 0;
int ballHitFly = 0;
int score = 0;
int miss = 0;
int lastTouchY = 120;

int batX = 0, batY = 0;
int prevBatX = -1, prevBatY = -1;
//============================
int base[3] = {0};
int playerScore = 0;
int lastHitType = 0;

extern int legendUnlocked;
int useLegendBatter = 1;
//============================
typedef enum {
    MODE_PRACTICE,
    MODE_PK,
    MODE_HR_DERBY
} GameMode;
GameMode currentMode = MODE_PRACTICE;
//============================

int hrPitchCount = 0; 
int hrScore      = 0;
int hrSpecial    = 0;  

//============================
typedef enum { GS_PRACTICE, GS_MENU, GS_PK, GS_HR } GameState;
GameState gState = GS_PRACTICE;
//============================
void DrawRGB565Image(int x, int y, const uint16_t *img, int w, int h) {
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            uint16_t color = img[j * w + i];
            LCD_DrawPixel(x + i, y + j, color);
        }
    }
}
//============================
void DrawPixelFieldBackground(void)
{

    LCD_SetTextColor(BLACK);
    LCD_FillRect(0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    for (int y = 200; y < BSP_LCD_GetYSize(); y += 10) {
        LCD_SetTextColor((y / 10) % 2 == 0 ? DARKGREEN : GREEN);
        LCD_FillRect(0, y, BSP_LCD_GetXSize(), 10);
    }
    LCD_SetTextColor(GRAY);
    LCD_FillRect(0, 40, BSP_LCD_GetXSize(), 5);

    LCD_SetTextColor(WHITE);
    LCD_FillRect(12, 115, 6, 6);


    for (int i = 0; i < 100; ++i) {
        int x = rand() % BSP_LCD_GetXSize();
        int y = rand() % 30;
        LCD_DrawPixel(x, y, LIGHTGRAY);
    }
}
//=============================
void ResetCommonVars(void)
{
    score = miss = hitCount = 0;
    base[0] = base[1] = base[2] = 0;
    playerScore = lastHitType = 0;
	  computerScore = 0;
    pkButtonDrawn = 0;
    swingFlag = ballInZone = ballAlreadyHit = ballHitFly = 0;
	  hrPitchCount = 0;
    hrScore      = 0;
    hrSpecial    = 0;
	  playerOut = 0;
}
//=============================
void ShowPKModeIntro(void)
{
    uint16_t screenWidth  = BSP_LCD_GetXSize();
    uint16_t screenHeight = BSP_LCD_GetYSize();

    LCD_SetTextColor(BLUE);
    LCD_FillRect(0, 0, screenWidth, screenHeight / 2);

    LCD_SetTextColor(YELLOW);
    LCD_FillRect(0, screenHeight / 2, screenWidth, screenHeight / 2);

    LCD_SetBackColor(BLUE);
    LCD_SetTextColor(WHITE);
    LCD_SetFont(&Font20);
    LCD_DisplayStringLineCol(2, 5, "   PK MODE     ");

    LCD_SetBackColor(BLUE);
    LCD_SetTextColor(WHITE);
    LCD_DisplayStringLineCol(4, 3, "Fubon Guardians vs");

    LCD_SetBackColor(YELLOW);
    LCD_SetTextColor(BLACK);
    LCD_DisplayStringLineCol(8, 5, " CTBC Brothers ");

    char lvlBuf[20];
    sprintf(lvlBuf, "Level  %d", opponentLevel + 1);
    LCD_SetBackColor(BLUE);
    LCD_SetTextColor(WHITE);
    LCD_SetFont(&Font16);
    LCD_DisplayStringLineCol(1, 20, lvlBuf);

    delay_ms(3000);

    LCD_SetFont(&Font16);
    LCD_SetBackColor(BLACK);
    LCD_SetTextColor(WHITE);
}
void InitPKMode(void)
{
	  practiceMode = 0;
    ShowPKModeIntro();
	  currentBall.active = 0;
    LCD_Clear(BLACK);
    ResetCommonVars();
	  waitAfterSwitch = 1;
	  if (legendUnlocked) useLegendBatter = 1;
    else  useLegendBatter = 0;
}
void ShowPKEndOptions(int playerWin)
{
  while (currentBall.active) {
    UpdateBall();
    delay_ms(20);
  }

  LCD_Clear(BLACK);
  LCD_SetFont(&Font24);
  LCD_SetTextColor(WHITE);

  if (playerWin) {
    LCD_DisplayStringLineCol(2, 3, "   YOU WIN    ");
    LCD_SetFont(&Font20);
    LCD_DisplayStringLineCol(6, 3, "PK high");
    LCD_DisplayStringLineCol(7, 3, "level team?");
    
// Draw YES/NO buttons
    LCD_SetTextColor(BLUE);   LCD_FillRect(40, 190, 100, 40);
    LCD_SetTextColor(RED);    LCD_FillRect(180,190, 100, 40);
    LCD_SetTextColor(WHITE);  LCD_SetBackColor(BLUE);
    LCD_DisplayStringAt(62, 204,  (uint8_t*)"YES", LEFT_MODE);
    LCD_SetBackColor(RED);
    LCD_DisplayStringAt(202,204,  (uint8_t*)"NO",  LEFT_MODE);

    TS_StateTypeDef ts;
    while (1) {
        TS_GetState(&ts);
        if (ts.TouchDetected && ts.y >= 190 && ts.y <= 230) {
            if (ts.x >= 40 && ts.x <= 140) {
                if (opponentLevel < 3) opponentLevel++;
                InitPKMode();
                gState = GS_PK;
                return;
            }
            else if (ts.x >= 180 && ts.x <= 280) {
                gState = GS_PRACTICE;
                practiceMode = 1;
                ResetCommonVars();
                return;
            }
        }
        delay_ms(50);
    }
  } else {
    LCD_DisplayStringLineCol(2, 3, "COMPUTER WIN");
    delay_ms(2000);

    gState = GS_PRACTICE;
    practiceMode = 1;
    ResetCommonVars();
		LCD_SetFont(&Font16);
    return;
  }
}

//=============================

static void DrawPitcher(int x, int y)
{
    for (int j = 0; j < 11; ++j)
        for (int i = 4; i < 16; ++i)
            LCD_DrawPixel(x + i, y + j, LIGHTBLUE);
    for (int j = 3; j < 7; ++j) {
        for (int i = 1; i < 4; ++i)  LCD_DrawPixel(x + i,  y + j, CYAN);
        for (int i = 16; i < 19; ++i) LCD_DrawPixel(x + i, y + j, CYAN);
    }

    for (int j = 11; j < 23; ++j)
        for (int i = 5; i < 15; ++i)
            LCD_DrawPixel(x + i, y + j, WHITE);
    for (int j = 18; j < 23; ++j)
        for (int i = 5; i < 9;  ++i)
            LCD_DrawPixel(x + i, y + j, GRAY);
    for (int i = 4;  i < 10; ++i) LCD_DrawPixel(x + i, y + 23, DARKGRAY);
    for (int i = 10; i < 16; ++i) LCD_DrawPixel(x + i, y + 23, DARKGRAY);
    for (int j = -4; j < 0; ++j)
        for (int i = 7; i < 13; ++i)
            LCD_DrawPixel(x + i, y + j, 0xFD20);

    for (int j = 0; j < 3; ++j)
        for (int i = 8; i < 12; ++i)
            LCD_DrawPixel(x + i, y + j, 0xFD20);

    /* ???? */
    for (int j = 5; j < 11; ++j)
        for (int i = 18; i < 20; ++i)
            LCD_DrawPixel(x + i, y + j, 0xFD20);
    for (int j = 6; j <= 9; ++j)
        for (int i = 18; i <= 20; ++i)
            LCD_DrawPixel(x + i, y + j, WHITE);

    LCD_DrawPixel(x + 18, y + 8, RED);
    LCD_DrawPixel(x + 20, y + 8, RED);

    for (int i = 7; i < 13; ++i) LCD_DrawPixel(x + i, y - 6, CYAN);
    for (int i = 6; i < 14; ++i) LCD_DrawPixel(x + i, y - 4, CYAN);

    for (int j = 8; j < 12; ++j)
        for (int i = -1; i < 3; ++i)
            LCD_DrawPixel(x + i, y + j, BROWN);
}

//=============================
int JudgeHitType(int ballY, int swingY) {
    int diff = abs(ballY - swingY);
    if (diff <= 2) return 3;
    else if (diff <= 8) return 2;
    else return 1;
}

void UpdateBases(int hitType) {
    int new_base[3] = {0};
    if (hitType == 3) {
        playerScore += base[0] + base[1] + base[2];
        new_base[2] = 1;
    } else if (hitType == 2) {
        playerScore += base[1] + base[2];
        new_base[1] = 1;
        new_base[2] = base[0];
    } else {
        playerScore += base[2];
        new_base[0] = 1;
        new_base[1] = base[0];
        new_base[2] = base[1];
    }
    base[0] = new_base[0];
    base[1] = new_base[1];
    base[2] = new_base[2];
}

void DrawBases() {
    int radius = 6;
    int spacing = 20;
    int baseY = BSP_LCD_GetYSize() - 15;
    int startX = BSP_LCD_GetXSize() - (spacing * 3 + 10);
    LCD_SetTextColor(WHITE);
    LCD_DisplayStringLineCol(12, 20, "Bases:");
    LCD_SetTextColor(base[0] ? RED : DARKGRAY);
    LCD_FillCircle(startX, baseY, radius);
    LCD_SetTextColor(base[1] ? RED : DARKGRAY);
    LCD_FillCircle(startX + spacing, baseY, radius);
    LCD_SetTextColor(base[2] ? RED : DARKGRAY);
    LCD_FillCircle(startX + spacing * 2, baseY, radius);
}

void DrawStartPKButton(void) {
    uint16_t btn_x = BSP_LCD_GetXSize() - 90;
    uint16_t btn_y = 10;
    uint16_t btn_w = 80;
    uint16_t btn_h = 40;
    LCD_SetTextColor(GREEN);
    LCD_FillRect(btn_x, btn_y, btn_w, btn_h);
    uint16_t text_x = btn_x + (btn_w - 6 * 5) / 2;
    uint16_t text_y = btn_y + (btn_h - 16) / 2;
    LCD_SetTextColor(WHITE);
    LCD_DisplayStringAt(text_x - 10, text_y, (uint8_t *)"START", LEFT_MODE);
}

uint8_t IsStartPKPressed(uint16_t x, uint16_t y) {
    uint16_t btn_x = BSP_LCD_GetXSize() - 90;
    uint16_t btn_y = 10;
    return (x >= btn_x && x <= btn_x + 80 && y >= btn_y && y <= btn_y + 40);
}
void DrawBat(int show, int x, int y) {
    if (prevBatX >= 0 && prevBatY >= 0) {
        LCD_SetTextColor(BLACK);
        LCD_FillCircle(prevBatX, prevBatY, BAT_RADIUS);
    }
    if (show) {
        LCD_SetTextColor(YELLOW);
        LCD_FillCircle(x, y, BAT_RADIUS);
        
        prevBatX = x;
        prevBatY = y;
    } else {
        
        prevBatX = -1;
        prevBatY = -1;
    }
}
//================================
//redraw text
void RedrawFixedTexts(void) {
    if (practiceMode) {
        char status[32];
        sprintf(status, "Score:%d Miss:%d", score, miss);
        LCD_SetTextColor(CYAN);
        LCD_DisplayStringLineCol(2, 0, status);
    } else if(gState == GS_PK){
        char status[32];
        sprintf(status, "FG:%d  CB:%d", playerScore, computerScore);
        LCD_SetTextColor(CYAN);
        LCD_DisplayStringLineCol(2, 0, status);
        char text[32];
        if (lastHitType == 1) sprintf(text, "1B HIT");
        else if (lastHitType == 2) sprintf(text, "2B HIT");
        else if (lastHitType == 3) sprintf(text, "3B HIT");
        else sprintf(text, "");
        LCD_SetTextColor(MAGENTA);
        LCD_DisplayStringLineCol(12, 10, text);
    }
    if (gState == GS_PK) DrawBases();
}
//=================================
//field element
void DrawFieldElements(void) {
	  DrawPixelFieldBackground();
    LCD_SetTextColor(RED);
    LCD_DrawRect(STRIKEZONE_X, STRIKEZONE_Y, STRIKEZONE_W, STRIKEZONE_H);
	  DrawRGB565Image(300, 100, batter, 25, 40);
	  DrawPitcher(10,110);
    LCD_SetTextColor(WHITE);
    LCD_DisplayStringLineCol(10, 0, "P");
    if (practiceMode) {
        char status[32];
        sprintf(status, "Score:%d Miss:%d", score, miss);
        LCD_SetTextColor(CYAN);
        LCD_DisplayStringLineCol(2, 0, status);
    } else if (gState == GS_PK) { 
      char status[32];
      sprintf(status, "FG:%d  CB:%d", playerScore, computerScore);
      LCD_SetTextColor(CYAN);
      LCD_DisplayStringLineCol(2, 0, status);

      char text[32];
      if      (lastHitType == 1) sprintf(text, "1B HIT");
      else if (lastHitType == 2) sprintf(text, "2B HIT");
      else if (lastHitType == 3) sprintf(text, "3B HIT");
      else                       sprintf(text, "");
      LCD_SetTextColor(MAGENTA);
      LCD_DisplayStringLineCol(12, 10, text);

    } else if (gState == GS_HR) { 
      char info[32];
      sprintf(info, "HR %d / 15", hrPitchCount);
      LCD_SetTextColor(YELLOW);
      LCD_DisplayStringLineCol(2, 0, info);
    }
}
//===============================
//new pitch
void StartNewPitch(void) {
	
    currentBall.x = 25;
    currentBall.y = 110 + (rand() % 10);
    currentBall.active = 1;
	
    currentBall.type = rand() % 9;
	
	  if (gState == GS_HR) {
			
      currentBall.type = FASTBALL;
      currentBall.vx   = 12;
      currentBall.vy   = 0;
      hrSpecial = ((hrPitchCount+1) % 5 == 0);
      hrPitchCount++; 
    }

		LCD_Clear(BLACK);
    DrawFieldElements();
    if (gState == GS_PK) DrawBases();
		if (practiceMode && hitCount >= 2) DrawStartPKButton();
		
    switch (currentBall.type) {
        case FASTBALL: currentBall.vx = 15;currentBall.vy = 0; 
			       if(gState == GS_HR) currentBall.vx = 12;
			       break;
        case CHANGEUP: currentBall.vx = 6; currentBall.vy = 0;break;
        case PHANTOM:  currentBall.vx = 5; currentBall.vy = 0;break;
			  case CURVEBALL:  currentBall.vx = 8 ; currentBall.vy = -4; break;
			  case SINKER:  currentBall.vx = 11 ;  currentBall.vy = 0; break;
			  case SNAKE: currentBall.vx = 9;currentBall.vy = 0 ;snakeFrame = 0;break;
			  case FLOAT:currentBall.vx = 10;currentBall.vy = 0;floatframe = 0;break;
        case reverse:currentBall.vx = 15;currentBall.vy = 0;break;			
        default:currentBall.vx = 9; currentBall.vy = 0;break;
    }
    LCD_Clear(BLACK);
    DrawFieldElements();
    if (gState == GS_PK) DrawBases();
    if (practiceMode && hitCount >= 2) {
        DrawStartPKButton();
    }
    if (practiceMode) {
        LCD_SetTextColor(WHITE);
        char text[20];
        sprintf(text, "Pitch: %s",
            (currentBall.type == FASTBALL) ? "FASTBALL" :
            (currentBall.type == CHANGEUP) ? "CHANGEUP" :
			      (currentBall.type == CURVEBALL) ? "CURVEBALL" :
			      (currentBall.type == SINKER) ? "SINKER" :
			      (currentBall.type == SNAKE) ? "SNAKE" :
			      (currentBall.type == FLOAT) ? "FLOAT" :
			      (currentBall.type == reverse) ? "REVERSE" :
            (currentBall.type == PHANTOM) ? "PHANTOM" : "STRAIGHT");
        LCD_DisplayStringLineCol(1, 0, text);
    }
} 
//=======================================
//update the ball,pitch and hit
void UpdateBall(void)
{
    static int prevX = -1, prevY = -1;

    if (prevX >= 0 && prevX < BSP_LCD_GetXSize() &&
        prevY >= 0 && prevY < BSP_LCD_GetYSize()) {
        LCD_SetTextColor(BLACK);
        LCD_FillCircle(prevX, prevY, BALL_RADIUS);
    }

    switch (currentBall.type) {
        case CURVEBALL:
            if (currentBall.x > 130) currentBall.vy += 1;
            break;
        case SINKER:
            if (currentBall.x > 180) currentBall.vy += 1;
            break;
        case FLOAT:
            if (!ballHitFly)
                currentBall.vy = (int)(FLOAT_AMP * sinf(FLOAT_FREQ * floatframe++));
            break;
        case SNAKE:
            if (!ballHitFly)
                currentBall.vy = (int)(SNAKE_AMPLITUDE * sinf(SNAKE_FREQ * snakeFrame++)) - (currentBall.y - 120);
            break;
        default:
            break;
    }

    if (currentBall.type == CHANGEUP && currentBall.x > 150 && !ballHitFly)
        currentBall.vx = 18;
    if (currentBall.type == reverse && currentBall.x > 200 && !ballHitFly)
        currentBall.vx = 5;

    if (ballHitFly) currentBall.vy += 1;
		int temp = currentBall.vx;
		currentBall.vx = (int)(currentBall.vx * speedMulTbl[opponentLevel]);

    currentBall.x += currentBall.vx;
    currentBall.y += currentBall.vy;
		currentBall.vx = temp;

    int show = 1;
    if (currentBall.type == PHANTOM) {
        if(currentBall.x < 85 || currentBall.x > 230){
				
				  show = 1;
					
				}else{
				
				  show = 0;
					
				}
    }
    if (show &&
    currentBall.x >= 0 && currentBall.x < BSP_LCD_GetXSize() &&
    currentBall.y >= 0 && currentBall.y < BSP_LCD_GetYSize()){
			
    if (useLegendBatter && ballHitFly && (gState == GS_HR || gState == GS_PK)){
			
        //legend hitter effect
			
        static uint16_t trailColors[] = { RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA };
        static int colorIdx = 0;

        LCD_SetTextColor(trailColors[colorIdx]);
        LCD_FillCircle(currentBall.x, currentBall.y, BALL_RADIUS + 1);
        colorIdx = (colorIdx + 1) % (sizeof(trailColors) / sizeof(trailColors[0]));
				
      }else if (gState == GS_HR && ballHitFly){
				
				//HR derby effect
        LCD_SetTextColor(RED);
        LCD_FillCircle(currentBall.x, currentBall.y, BALL_RADIUS + 1);
        LCD_SetTextColor(ORANGE);
        LCD_FillCircle(currentBall.x, currentBall.y, BALL_RADIUS);
        LCD_SetTextColor(YELLOW);
        LCD_FillCircle(currentBall.x, currentBall.y, BALL_RADIUS - 1);
				
      }else{
				
        LCD_SetTextColor(WHITE);
        LCD_FillCircle(currentBall.x, currentBall.y, BALL_RADIUS);
				
      }
    }

    prevX = currentBall.x;
    prevY = currentBall.y;

    int in_zone_x = currentBall.x >= STRIKEZONE_X - HIT_MARGIN &&
                    currentBall.x <= STRIKEZONE_X + STRIKEZONE_W + HIT_MARGIN;
    int in_zone_y = currentBall.y >= STRIKEZONE_Y - HIT_MARGIN &&
                    currentBall.y <= STRIKEZONE_Y + STRIKEZONE_H + HIT_MARGIN;
    ballInZone = in_zone_x && in_zone_y;

    if (ballInZone && swingFlag && !ballAlreadyHit) {
			
        int dx = currentBall.x - batX;
        int dy = currentBall.y - batY;
        int dist2 = dx * dx + dy * dy;
        int hit_range = (BALL_RADIUS + BAT_RADIUS + HIT_MARGIN);
        if (dist2 <= hit_range * hit_range) {
            LCD_DisplayStringLineCol(12, 0, "HIT!");
            score++;
            if (practiceMode) hitCount++;
            else {
                lastHitType = JudgeHitType(currentBall.y, batY);
                UpdateBases(lastHitType);
            }
            ballAlreadyHit = 1;
            ballHitFly = 1;
            if(useLegendBatter && ballHitFly && (gState == GS_HR || gState == GS_PK)){
							  
							  currentBall.vx = -(15 + rand() % 3);
                currentBall.vy = -(13 + rand() % 3);
							
						}else if (gState == GS_HR) {
							
                currentBall.vx = -(12 + rand() % 3);
                currentBall.vy = -(10 + rand() % 3);
            } else {
							
                currentBall.vx = -(9 + rand() % 3);
                currentBall.vy = -(9 + rand() % 3);
            }
        }
    }
    if (swingTimer > 0) {
        swingFlag = 1;
        swingTimer--;
        DrawBat(1, batX, batY);
    } else {
        if (swingFlag) DrawBat(0, batX, batY);
        swingFlag = 0;
    }
    if (ballHitFly && (currentBall.x < 0 || currentBall.y > BSP_LCD_GetYSize())) {
        if (gState == GS_HR && ballAlreadyHit) {
            int isHR = (currentBall.x < 0 && currentBall.y < STRIKEZONE_Y + 20);
            if (isHR) {
                int add = hrSpecial ? 3 : 1;
                hrScore += add;
                LCD_SetTextColor(GREEN);
                char msg[20];
                sprintf(msg, "HOMERUN +%d!", add);
                LCD_DisplayStringLineCol(1, 0, msg);
            } else {
                LCD_SetTextColor(CYAN);
                LCD_DisplayStringLineCol(1, 0, "INFIELD");
            }
            delay_ms(800);
        }

        LCD_SetTextColor(BLACK);
        LCD_FillCircle(currentBall.x, currentBall.y, BALL_RADIUS);
        RedrawFixedTexts();
        currentBall.active = 0;
        ballHitFly = 0;
        ballInZone = 0;
        swingFlag = 0;
        ballAlreadyHit = 0;
        LCD_Clear(BLACK);
    }

    if (!ballHitFly && currentBall.x > STRIKEZONE_X + STRIKEZONE_W + HIT_MARGIN) {
        if (!ballAlreadyHit) {
					  LCD_SetTextColor(WHITE);
            LCD_DisplayStringLineCol(12, 0, "MISS...");
            if (practiceMode) miss++;
            else {
                playerOut++;
                if (playerOut >= 3) showSwitchMessage = 1;
            }
        }
        delay_ms(600);
        LCD_Clear(BLACK);
        currentBall.active = 0;
        swingFlag = 0;
        ballInZone = 0;
        ballAlreadyHit = 0;
    }
}

//====================================
//Swing
void PlayerSwing(int touchX, int touchY) {
    swingTimer = SWING_DURATION;
    batX = touchX;
    batY = touchY;
    if (touchY >= 0 && touchY < BSP_LCD_GetYSize()) {
        lastTouchY = touchY;
    }
    if (!ballInZone && currentBall.active && !ballHitFly && !ballAlreadyHit) {
        LCD_DisplayStringLineCol(12, 0, "MISS (Too Early)");
        if (practiceMode) miss++;
        else {
            playerOut++;
            if (playerOut >= 3) showSwitchMessage = 1;
        }
        delay_ms(500);
        LCD_Clear(BLACK);
        currentBall.active = 0;
        swingFlag = 0;
        ballInZone = 0;
        ballAlreadyHit = 0;
    }
}
//=======================================
//Menu
void ShowGameModeMenu(void)
{
    LCD_Clear(BLACK);
    LCD_SetBackColor(BLACK);
    LCD_SetTextColor(WHITE);
    LCD_SetFont(&Font20);
    LCD_DisplayStringLineCol(3, 4, "SELECT MODE");
    LCD_SetTextColor(BLUE);
    LCD_FillRect(60, 80, 200, 40);
    LCD_SetTextColor(WHITE);
    LCD_DisplayStringAt(100, 92, (uint8_t *)"PK MODE", LEFT_MODE);
    LCD_SetTextColor(RED);
    LCD_FillRect(60, 140, 200, 40);
    LCD_SetTextColor(WHITE);
    LCD_DisplayStringAt(82, 156, (uint8_t *)"HR DERBY", LEFT_MODE);
}
void InitHRDerby(void)
{ 
	  practiceMode = 0;
    LCD_Clear(BLACK);
	  currentBall.active = 0;
    LCD_SetTextColor(YELLOW);
    LCD_SetFont(&Font24);
    LCD_DisplayStringLineCol(5, 2, "HR  DERBY!");
	  LCD_SetFont(&Font16);
    delay_ms(2000);
    LCD_Clear(BLACK);
	  currentBall.active = 0;
    ResetCommonVars();
	  waitAfterSwitch = 1;
	  if (legendUnlocked) useLegendBatter = 1;
    else                useLegendBatter = 0;
}

//===================================
//Main geme-loop
void GameLoop(void)
{
    TS_StateTypeDef ts;
    TS_GetState(&ts);
    if (ts.TouchDetected && (gState == GS_PRACTICE || gState == GS_PK || gState == GS_HR))
        PlayerSwing(ts.x, ts.y);
    switch (gState)
    {
    case GS_PRACTICE:
			  practiceMode = 1;
        if (!currentBall.active) StartNewPitch();
        else                     UpdateBall();


        if (hitCount >= 2 && !pkButtonDrawn) { DrawStartPKButton(); pkButtonDrawn = 1; }


        if (hitCount >= 2 && pkButtonDrawn && ts.TouchDetected && IsStartPKPressed(ts.x, ts.y)) {
            gState = GS_MENU;
            ShowGameModeMenu();
        }
        break;
    case GS_MENU:
        if (ts.TouchDetected) {
            if (ts.y >= 80 && ts.y <= 120) {         
                gState = GS_PK;
                InitPKMode();
            }
            else if (ts.y >= 140 && ts.y <= 180) {   
                gState = GS_HR;
                InitHRDerby();
            }
        }
        break;
				
    case GS_PK:
        if (showSwitchMessage) {
            LCD_Clear(BLACK);
            LCD_SetTextColor(WHITE);
            LCD_SetFont(&Font24);
            LCD_DisplayStringLineCol(4, 2, "SWITCH INNING");
            LCD_SetFont(&Font16);
            delay_ms(2000);

            int compRun = 1 + rand() % compRunTbl[opponentLevel];
            computerScore += compRun;
            LCD_SetTextColor(BLACK);
            LCD_Clear(YELLOW);
            LCD_SetBackColor(YELLOW);
            char compMsg[32];
            sprintf(compMsg, "Computer scores %d!", compRun);
            LCD_DisplayStringLineCol(6, 5, compMsg);
            delay_ms(2000);
            LCD_SetTextColor(WHITE);
            LCD_SetBackColor(BLACK);

            playerOut = 0;
            hitCount  = 0;
            base[0]   = base[1] = base[2] = 0;
            showSwitchMessage = 0;
            waitAfterSwitch   = 1;
            LCD_Clear(BLACK);
            break;
        }

        if (waitAfterSwitch) {
            DrawFieldElements();
            DrawBases();
            delay_ms(1000);
            waitAfterSwitch = 0;
            break;
        }
				if (playerScore >= 10 || computerScore >= 10) {
            ShowPKEndOptions(playerScore >= 10);
            ResetCommonVars();
        }
        if (!currentBall.active) StartNewPitch();
        else                     UpdateBall();
        break;

    case GS_HR:
			
        if (hrPitchCount >= 15 && !currentBall.active) {
            char msg[32];
            sprintf(msg, "HRs: %d", hrScore);
            LCD_SetTextColor(YELLOW);
            LCD_Clear(BLACK);
            LCD_DisplayStringLineCol(6, 4, "DERBY OVER!");
            LCD_DisplayStringLineCol(8, 4, msg);
            delay_ms(4000);
            gState = GS_PRACTICE;
            practiceMode = 1;
            ResetCommonVars();
            break;
        }

        if (!currentBall.active)
            StartNewPitch();
        else
            UpdateBall();
        break;
				
				}
}

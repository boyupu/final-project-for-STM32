#ifndef __GAME_H
#define __GAME_H

typedef enum {
    FASTBALL,
    CHANGEUP,   // Slower start, speeds up near strike zone
    PHANTOM,    // Invisible during flight, visible near plate
    STRAIGHT,
	  CURVEBALL,    
    SINKER,
    SNAKE,
	  FLOAT,
	  reverse
} PitchType;

typedef struct {
    int x, y;
    int vx, vy;
    PitchType type;
    int active;
} Ball;

void StartNewPitch(void);
void UpdateBall(void);
void PlayerSwing(int touchX,int touchY);
void DrawBall(void);
void GameLoop(void);

#endif

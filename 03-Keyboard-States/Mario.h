#pragma once
#include "GameObject.h"

#define MARIO_JUMP_SPEED_Y		0.8f
#define MARIO_GRAVITY		0.0021875f
#define MARIO_AIR		0.21875f

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_WALKING_DECELERATION 0.3f
#define MARIO_WALKING_ACCELEROMETER 0.016875f
#define MARIO_MAX_SPEED 0.2f
#define FRICTION 0.0046875f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RUNNING			400

#define MARIO_ANI_IDLE_RIGHT		0
#define MARIO_ANI_IDLE_LEFT			1
#define MARIO_ANI_WALKING_RIGHT		2
#define MARIO_ANI_WALKING_LEFT		3
#define MARIO_ANI_WALKING_STOP_LEFT		4
#define MARIO_ANI_WALKING_STOP_RIGHT	5

class CMario : public CGameObject
{

	bool isRunning = false;
	float vx_acceleration = MARIO_WALKING_ACCELEROMETER;
	float vy_acceleration = MARIO_WALKING_ACCELEROMETER;
	bool isOnGround = true;
	DWORD T_draw=0;
	DWORD T_draw_end;
	int ani;
	bool onRollBack = false;
public: 

	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};
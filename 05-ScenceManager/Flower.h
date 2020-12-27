#pragma once
#include "Enemy.h"

#define FLOWER_BBOX_WIDTH								16.0f
#define FLOWER_BBOX_HEIGHT								32.0f

#define FLOWER_SHOOT_SPEED_X							0.1f


#define FLOWER_TYPE_RED_FLOWER_FIRE						1
#define FLOWER_TYPE_GREEN_FLOWER_FIRE					2
#define FLOWER_TYPE_PIRANHA_FLOWER_FIRE					3

#define FLOWER_STATE_UP									0
#define FLOWER_STATE_DOWN								1
#define FLOWER_STATE_AIM_TARGET							2


#define FLOWER_ANI_UP_MOVE_OPEN_MOUTH_LEFT				0
#define FLOWER_ANI_UP_OPEN_MOUTH_LEFT					1

#define FLOWER_ANI_DOWN_MOVE_CLOSE_MOUTH_LEFT			2
#define FLOWER_ANI_DOWN_OPEN_MOUTH_LEFT					3

#define FLOWER_ANI_UP_MOVE_OPEN_MOUTH_RIGHT				4
#define FLOWER_ANI_UP_OPEN_MOUTH_RIGHT					5

#define FLOWER_ANI_DOWN_MOVE_CLOSE_MOUTH_RIGHTH			6
#define FLOWER_ANI_DOWN_OPEN_MOUTH_RIGHT				7

///////////////////// FLOWRE GREEN HAVE FIRE BALL
#define FLOWER_GREEN_ANI_UP_MOVE_OPEN_MOUTH_LEFT				8
#define FLOWER_GREEN_ANI_UP_OPEN_MOUTH_LEFT						9

#define FLOWER_GREEN_ANI_DOWN_MOVE_CLOSE_MOUTH_LEFT				10
#define FLOWER_GREEN_ANI_DOWN_OPEN_MOUTH_LEFT					11

#define FLOWER_GREEN_ANI_UP_MOVE_OPEN_MOUTH_RIGHT				12
#define FLOWER_GREEN_ANI_UP_OPEN_MOUTH_RIGHT					13

#define FLOWER_GREEN_ANI_DOWN_MOVE_CLOSE_MOUTH_RIGHTH			14
#define FLOWER_GREEN_ANI_DOWN_OPEN_MOUTH_RIGHT					15


#define FLOWER_GREEN_PIRANHA_ANI_UNFIRE							16

class CFlower :
    public Enemy
{

public:
	bool priority = false;
	bool isWaitingShooting = false;
	bool isShooting = false;

	int typeFlower = FLOWER_TYPE_RED_FLOWER_FIRE;
	int ny = 0;

	int	  HeightPipe;
	float start_y;

	DWORD delayBullet = 0;
	DWORD setInterval = 0;
	DWORD setInterval2 = 0;
	CFlower(int x ,int y,int, int typ = FLOWER_TYPE_RED_FLOWER_FIRE);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);

	void setTimeLoadingBullet() { delayBullet = GetTickCount64(); }
	bool isReadyFire() { return GetTickCount64() - delayBullet > 1500; }

};


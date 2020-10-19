#pragma once
#include "GameObject.h"

#define FLOWER_BBOX_WIDTH  32.0f
#define FLOWER_BBOX_HEIGHT 32.0f

#define FLOWER_STATE_UP		0
#define FLOWER_STATE_DOWN	1

class CFlower :
    public CGameObject
{

public:
	bool priority = false;
	bool isDelayedShooting = true;
	DWORD delayBullet = 0;
	DWORD setInterval = 0;
	DWORD setInterval2 = 0;
	//CFlower();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);

};


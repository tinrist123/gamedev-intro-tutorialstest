#pragma once
#include "GameObject.h"

#define BULLET_SPEED_X				0.05f;
#define BULLET_SPEED_Y				0.05f;
#define BULLET_START_SPEED_Y		0.0f;

class FireBullet :
    public CGameObject
{
public:
	int ny;
	//CBrick(int width, int height);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


#pragma once
#include "MarioWeapon.h"


#define BULLET_SPEED_X				0.15f;
#define BULLET_SPEED_Y				0.15f;
#define BULLET_START_SPEED_Y		0.0f;

#define BULLET_STATE_NORMAL_MOVING	1

#define BULLET_BBOX					8	
class MarioBullet : 
	public MarioWeapon
{
public:
	int pointCollisionX;
	int pointCollisionY;


	int ny;
	MarioBullet();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
	virtual void Render();
	
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetUpAddingEffectCollision();
};


#pragma once
#include "MarioWeapon.h"
class MarioTail :
	public MarioWeapon
{
public:
	int pointCollisionX;
	int pointCollisionY;

	int ny;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

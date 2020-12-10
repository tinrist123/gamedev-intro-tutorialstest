#pragma once
#include "GameObject.h"
class MarioWorldMap : public CGameObject
{
	public:
	int ny = 0;
	DWORD delayBullet = 0;
	DWORD setInterval = 0;
	DWORD setInterval2 = 0;
	MarioWorldMap();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
};


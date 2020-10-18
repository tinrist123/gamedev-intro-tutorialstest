#pragma once
#include "GameObject.h"

#define FLOWER_BBOX_WIDTH  32.0f
#define FLOWER_BBOX_HEIGHT 55.0f

class CFlower :
    public CGameObject
{

public:
	DWORD setInterval = 0;
	//CFlower();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt);
	void SetState(int state);
};


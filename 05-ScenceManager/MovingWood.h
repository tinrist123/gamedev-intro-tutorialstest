#pragma once
#include "Ground.h"

#define MOVING_WOOD_SPEED_X								0.03f

#define MOVING_WOOD_STATE_MOVING_HORIZONTAL				1	
#define MOVING_WOOD_STATE_FALLING						2	

class MovingWood: public Ground
{
public:
	MovingWood();
	~MovingWood();

	void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* staticObj);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


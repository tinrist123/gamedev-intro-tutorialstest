#pragma once
#include "GameObject.h"
#include "Mario.h"

#define CARRET_STATE_MOVE_UP			0
#define CARRET_STATE_MOVE_DOWN			1

class SelectionCarret : public CGameObject
{
public:
	CMario* mario;
public:
	SelectionCarret(CMario* mario);
	~SelectionCarret();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
};


#pragma once
#include "Item.h"

#define PSWITCH_ANI_UP			5
#define PSWITCH_ANI_CRUSHED		6

class P_Switch :public Item
{
public:
	bool isActived = false;

	P_Switch(float posX, float posY);
	P_Switch();
	~P_Switch();

	void SwitchActive() { isActived = true; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


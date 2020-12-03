#pragma once
#include "Item.h"

 class Coin : public Item
{
public:
	Coin(float posX, float posY);
	Coin();
	~Coin();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


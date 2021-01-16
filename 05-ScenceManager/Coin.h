#pragma once
#include "Item.h"


 class Coin : public Item
{
public:
	bool isTranformed_Coin = false;

	Coin(float posX, float posY);
	Coin();
	~Coin();

	void TranformedWeakBrickToCoin() { this->isTranformed_Coin = true; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


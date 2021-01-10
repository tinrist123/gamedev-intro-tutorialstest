#pragma once
#include "Item.h"

#define KIND_RED_MUSHROOM			1
#define KIND_GREEN_MUSHROOM			2

class Mushroom: public Item
{
public:
	bool onGravity = false;
	int kindOfMushroom = 1;
	Mushroom(float posX,float posY);
	~Mushroom();

	void setKindOfMushroom(int type) { this->kindOfMushroom = type; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


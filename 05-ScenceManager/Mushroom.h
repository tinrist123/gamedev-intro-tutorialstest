#pragma once
#include "Item.h"



class Mushroom: public Item
{
public:
	bool onGravity = false;

	Mushroom(float posX,float posY);
	~Mushroom();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


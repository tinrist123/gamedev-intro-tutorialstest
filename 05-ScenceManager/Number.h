#pragma once
#include "BoardAddSprite.h"
class Number : public BoardAddSprite
{
public:
	Number();
	void setPos(float posX, float posY) { this->x = posX; this->y = posY; }
	virtual void Render();
	//void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~Number();
};


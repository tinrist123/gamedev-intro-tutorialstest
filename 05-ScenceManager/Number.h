#pragma once
#include "BoardAddSprite.h"
class Number : public BoardAddSprite
{
public:
	Number();
	//virtual void Render();
	//void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


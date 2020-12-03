#pragma once
#include "BoardAddSprite.h"
class CarretSpeed : public BoardAddSprite
{
public:
	CarretSpeed();
	virtual void Render();
	void Update(float camX, float camY);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

typedef CarretSpeed * LPCARRET;
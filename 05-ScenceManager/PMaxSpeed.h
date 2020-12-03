#pragma once
#pragma message ("World object is defined")
#include "BoardAddSprite.h"
class PMaxSpeed : public BoardAddSprite
{
public:
	PMaxSpeed();
	virtual void Render();
	void Update(float camX, float camY);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


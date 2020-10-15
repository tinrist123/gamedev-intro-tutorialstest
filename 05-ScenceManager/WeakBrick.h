#pragma once
#include "Brick.h"
class WeakBrick :
    public CBrick
{
	int height;
	int width;
public:
	WeakBrick(int width, int height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


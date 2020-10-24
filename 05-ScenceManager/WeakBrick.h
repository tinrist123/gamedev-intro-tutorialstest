#pragma once
#include "Brick.h"

#define WEAK_BREAK_ANI	1	

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


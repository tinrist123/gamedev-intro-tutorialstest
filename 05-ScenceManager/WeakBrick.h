#pragma once
#include "Brick.h"

#define WEAK_BREAK_ANI	1	

class WeakBrick :
    public CBrick
{
	int height;
	int width;

	bool isHaveP_Swtich = false;
public:
	WeakBrick(int width, int height, int);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


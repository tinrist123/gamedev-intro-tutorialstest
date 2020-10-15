#pragma once
#include "Brick.h"
class CloudBrick : public CBrick
{
	int height;
	int width;
public:

	explicit CloudBrick(int width,	int height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


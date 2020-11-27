#pragma once
#include "Ground.h"

#define MARIO_ANI_CLOUD_BRICK		2
#define BRICK_BBOX_WIDTH			16
#define BRICK_BBOX_HEIGHT			16

class CBrick : public Ground
{
	int height;
	int width;
public:
	explicit CBrick(int width,int height);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
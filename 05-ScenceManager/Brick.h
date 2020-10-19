#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject
{
	int height;
	int width;
public:
	explicit CBrick(int width,int height);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
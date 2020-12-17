#pragma once
#include "GameObject.h"

#define BBOX_WIDTH  16
#define BBOX_HEIGHT 16

class Ground : public CGameObject
{
public:
	int height;
	int width;
public:
	Ground();
	Ground(int width, int height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


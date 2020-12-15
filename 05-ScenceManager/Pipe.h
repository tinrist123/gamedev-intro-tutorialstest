#pragma once
#define BBOX_WIDTH  16
#define BBOX_HEIGHT 16
#include "Ground.h"

class Pipe : public Ground
{
	int height;
	int width;
public:
	Pipe();
	Pipe(int width, int height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};



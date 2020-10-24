#include "Ground.h"

Ground::Ground(int width, int height)
{
	this->width = width;
	this->height = height;
}

void Ground::Render()
{
	//RenderBoundingBox();
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BBOX_WIDTH * width;
	b = y + BBOX_HEIGHT * height;
}
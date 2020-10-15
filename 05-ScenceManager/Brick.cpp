#include "Brick.h"
#include "../01-Skeleton/debug.h"



CBrick::CBrick(int width, int height)
{
	this->width = width;
	this->height = height;
}

void CBrick::Render()
{
	//animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH * width;
	b = y + BRICK_BBOX_HEIGHT * height;
}
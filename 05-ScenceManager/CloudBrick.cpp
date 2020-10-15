#include "CloudBrick.h"

CloudBrick::CloudBrick(int width, int height) : CBrick::CBrick(width,height)
{
	this->width = width;
	this->height = height;
}

void CloudBrick::Render()
{
	animation_set->at(2)->Render(x, y);
	//RenderBoundingBox();
}

void CloudBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH * width;
	b = y + BRICK_BBOX_HEIGHT * height;
}

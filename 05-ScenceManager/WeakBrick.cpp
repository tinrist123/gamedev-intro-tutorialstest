#include "WeakBrick.h"

WeakBrick::WeakBrick(int width, int height) : CBrick::CBrick(width, height)
{
	this->width = width;
	this->height = height;
}

void WeakBrick::Render()
{
	animation_set->at(WEAK_BREAK_ANI)->Render(x, y);
	//RenderBoundingBox();
}

void WeakBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH * width;
	b = y + BRICK_BBOX_HEIGHT * height;
}
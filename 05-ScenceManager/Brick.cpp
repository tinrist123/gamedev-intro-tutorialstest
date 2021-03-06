#include "Brick.h"
#include "Utils.h"



CBrick::CBrick()
{
	this->category = Category::BRICK;
}

CBrick::CBrick(int width, int height) : Ground(width,height)
{
	this->width = width;
	this->height = height;
	this->type = Type::CBRICK;
	this->category = Category::BRICK;
}

void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH * width;
	b = y + BRICK_BBOX_HEIGHT * height;
}
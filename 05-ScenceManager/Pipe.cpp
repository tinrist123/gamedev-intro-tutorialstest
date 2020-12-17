#include "Pipe.h"



Pipe::Pipe() : Ground()
{

}

Pipe::Pipe(int width, int height)
{
	this->width = width;
	this->height = height;
	this->type = Type::PIPE;
}

Pipe::Pipe(int width, int height,bool isHaveHiddenMap, bool isInHiddenMap, bool isPushMarioOut) : Ground(width, height)
{
	this->width = width;
	this->height = height;
	this->type = Type::PIPE;
	this->isHaveHiddenMap = isHaveHiddenMap;
	this->isInHiddenMap = isInHiddenMap;
	this->isPushMarioOut = isPushMarioOut;
}

void Pipe::Render()
{
	//animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BBOX_WIDTH * width;
	b = y + BBOX_HEIGHT * height;
}

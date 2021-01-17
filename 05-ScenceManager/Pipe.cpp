#include "Pipe.h"



Pipe::Pipe() : Ground()
{

}

Pipe::Pipe(int width, int height)
{
	this->width = width;
	this->height = height;
	this->type = Type::PIPE;
	this->category = Category::GROUND;
}

Pipe::Pipe(int width, int height,bool isHaveHiddenMap, bool isInHiddenMap, bool isPushMarioOut, int directionOyOfMario) : Ground(width, height)
{
	this->width = width;
	this->height = height;
	this->type = Type::PIPE;
	this->category = Category::GROUND;
	this->isHaveHiddenMap = isHaveHiddenMap;
	this->isInHiddenMap = isInHiddenMap;
	this->isPushMarioOut = isPushMarioOut;

	this->directionOy = directionOyOfMario;
}

void Pipe::Render()
{
	if (height == 1)
	{
		ani = 3;
	}
	else if (height == 2)
	{
		ani = 0;
	}
	else if (height == 4)
	{
		ani = 5;
	}
	else
	{
		ani = 1;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BBOX_WIDTH * width;
	b = y + BBOX_HEIGHT * height;
}

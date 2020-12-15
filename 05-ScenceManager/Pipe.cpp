#include "Pipe.h"



Pipe::Pipe() : Ground()
{

}

Pipe::Pipe(int width, int height) : Ground(width, height)
{
	this->width = width;
	this->height = height;
	this->type = 05;
}

void Pipe::Render()
{
	//animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BBOX_WIDTH * width;
	b = y + BBOX_HEIGHT * height;
}

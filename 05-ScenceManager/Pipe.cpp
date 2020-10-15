#include "Pipe.h"



Pipe::Pipe(int width, int height)
{
	this->width = width;
	this->height = height;
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

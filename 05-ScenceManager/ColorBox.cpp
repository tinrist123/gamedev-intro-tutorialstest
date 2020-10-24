#include "ColorBox.h"


ColorBox::ColorBox(int width, int height) : CGameObject()
{
	this->width = width;
	this->height = height;
}

void ColorBox::Render()
{
	//RenderBoundingBox();
}

void ColorBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BBOX_WIDTH*width;
	b = y + BBOX_HEIGHT*height;
}